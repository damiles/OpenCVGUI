#include "OGUIWindow.h"
#include "OGUIArea.h"
#include "OGUILayout.h"
#include "OGUIWidget.h"
#include "OGUIUtils.h"

#include <GL/glew.h>
#ifdef __APPLE__
#   define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
#include "nanovg.h"

#ifdef OGUI_GL3
#define NANOVG_GL3_IMPLEMENTATION
#else
#define NANOVG_GL2_IMPLEMENTATION
#endif

#include "nanovg_gl.h"

void errorcb(int error, const char* desc)
{
    printf("GLFW error %d: %s\n", error, desc);
}

namespace OpenCVGUI {

    int init(){
      if(!glfwInit()){
            printf("Failed init GLFW");
            return 0;
        }
    
        glfwSetErrorCallback(errorcb);

    #ifndef _WIN32 // don't require this on win32, and works with more cards
        #ifdef OGUI_GL3
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #else
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        #endif
    #endif
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

        
      } ;
    
bool OGUIWindow::isInitGlfw= false;

OGUIWindow::OGUIWindow(int width, int height,
                       const char* title,
                       int layoutOrientation,
                       bool fullScreen)
{
    this->width= width;
	this->height= height;
	this->title.append(title);

    init(fullScreen);
    
    this->mainLayout= new OGUILayout(this, layoutOrientation);
    this->mainLayout->title="Main  Layout";
    this->actual_cursor_type=0;
    this->key_focus_widget_= NULL;
    this->key_pressed= 0;
    this->_show_graph= false;
    this->_window_status= 1;
    this->_fileBrowser= new OGUIFileBrowser(this);
    this->_maximizedArea= NULL;
}

OGUIWindow::~OGUIWindow()
{
#ifdef OGUI_GL3
    nvgDeleteGL3((NVGcontext*)vg);
#else
    nvgDeleteGL2((NVGcontext*)vg);
#endif
	
    glfwDestroyWindow((GLFWwindow*)glfw_window);
    glfwTerminate();
}

int OGUIWindow::init(bool fullScreen)
{
    if(fullScreen) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        width= mode->width;
        height= mode->height;
        this->glfw_window = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, NULL);
    }else
	    this->glfw_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (!glfw_window) {
        fprintf(stderr, "Error creating glfw window\n");
        glfwTerminate();
        return 0;
	}

    initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");

    glfwSetWindowUserPointer((GLFWwindow*)glfw_window, this);
	glfwMakeContextCurrent((GLFWwindow*)glfw_window);
    glfwSwapInterval(0);
    
    // Init GLew
    glewExperimental = GL_TRUE;
    GLenum err= glewInit();
    if(err != GLEW_OK) {
        printf("Could not init glew.\n");
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 0;
    }
    // GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll consume it here.
    glGetError();

#ifdef OGUI_GL3
    vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#else
    vg = nvgCreateGL2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#endif    

    cout << glGetString(GL_VERSION) << endl;

    if(vg==NULL){
        fprintf(stderr, "Error: can not init nanovg");
        return 0;
    }

    nvgCreateFontMem((NVGcontext*)vg, "sans", font_roboto, font_roboto_size, 1);
    nvgCreateFontMem((NVGcontext*)vg, "sans-bold", font_valera, font_valera_size, 1);
    nvgCreateFontMem((NVGcontext*)vg, "icons", font_awesome, font_awesome_size, 1);

    // Load all cursors
    mouse_cursors_.push_back(glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
    mouse_cursors_.push_back(glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR));
    mouse_cursors_.push_back(glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR));
    mouse_cursors_.push_back(glfwCreateStandardCursor(GLFW_HAND_CURSOR));
    mouse_cursors_.push_back(glfwCreateStandardCursor(GLFW_IBEAM_CURSOR));
    mouse_cursors_.push_back(glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR));

    glfwSetInputMode((GLFWwindow*)glfw_window, GLFW_STICKY_MOUSE_BUTTONS, 1);

    glfwSetScrollCallback((GLFWwindow*)glfw_window, (void (*)(GLFWwindow*, double, double))OGUIWindow::scroll_callback);
    glfwSetCharCallback((GLFWwindow*)glfw_window, (void (*)(GLFWwindow*, unsigned int))OGUIWindow::charCallback);
    glfwSetKeyCallback((GLFWwindow*)glfw_window, (void (*)(GLFWwindow*,  int, int, int, int))OGUIWindow::keyCallback);

    glfwSetTime(0);
    prevt = glfwGetTime();

    return 1;
}

void OGUIWindow::scroll_callback(void* window, double xoffset, double yoffset){
    OGUIWindow* oguiwindow= (OGUIWindow*)glfwGetWindowUserPointer((GLFWwindow*)window);
    oguiwindow->mainLayout->updateScrollStatus(xoffset, yoffset);
}

void OGUIWindow::charCallback(void* window, unsigned int key)
{
    OGUIWindow* oguiwindow= (OGUIWindow*)glfwGetWindowUserPointer((GLFWwindow*)window);
    OGUIWidget* widget= oguiwindow->getKeyFocusWidget();
    if(widget!=NULL){
        widget->characterCallback(key);
    }
}

void OGUIWindow::keyCallback(void* window, int key, int scancode, int action, int mods)
{
    OGUIWindow* oguiwindow= (OGUIWindow*)glfwGetWindowUserPointer((GLFWwindow*)window);
    OGUIWidget* widget= oguiwindow->getKeyFocusWidget();
    if(widget!=NULL){
        widget->keyCallback(key, scancode, action, mods);
    }

    // Set the actual press key
    if( action == GLFW_PRESS || action == GLFW_REPEAT){
        oguiwindow->key_pressed= key;
    }else if( action == GLFW_RELEASE){
        oguiwindow->key_pressed= 0;
    }
}

void OGUIWindow::addArea(OGUIArea* area)
{
    mainLayout->addArea(area);
}

void OGUIWindow::updatePerfGraph()
{
    double t, dt;
    t = glfwGetTime();
    dt = t - prevt;
    prevt = t;
    updateGraph(&fps, dt);
}

void OGUIWindow::showPerfGraph(bool show)
{
    _show_graph=show;
}

bool OGUIWindow::update(){
    glfwMakeContextCurrent((GLFWwindow *)glfw_window);
    if(!glfwWindowShouldClose((GLFWwindow *)glfw_window)) {

        glfwGetCursorPos((GLFWwindow *) glfw_window, &mouse_x, &mouse_y);
        mouse_left_state = glfwGetMouseButton((GLFWwindow *) glfw_window, GLFW_MOUSE_BUTTON_LEFT);
        mouse_state = glfwGetMouseButton((GLFWwindow *) glfw_window, GLFW_MOUSE_BUTTON_LEFT);

        actual_cursor_type = 0;
        draw();
        drawCursor();

        glfwPollEvents();

        return true;
    }else{
        _popup_result= 0;
        _window_status= 0;
        return false;
    }
}

void OGUIWindow::draw()
{
    float ratio;
    
    // Get size and clean buffer
    glfwGetFramebufferSize((GLFWwindow*)glfw_window, &width, &height);
    ratio = width / (float) height;
    
    glViewport(0, 0, width, height);
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame((NVGcontext*)vg, width, height, ratio);

    if(_maximizedArea!=NULL){
        // Draw title
        NVGcontext* tvg= (NVGcontext*)(vg);
        nvgBeginPath(tvg);
        nvgRect(tvg, 0, 0, width, 22);
        nvgFillColor(tvg, nvgRGBA(0, 0, 0, 100));
        nvgFill(tvg);

        // Draw text
        nvgFontSize(tvg, 16.0f);
        nvgFontFace(tvg, "sans");
        float tw = nvgTextBounds(tvg, 0, 0, "Fullscreen Mode", NULL, NULL);
        nvgTextAlign(tvg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        nvgFillColor(tvg, nvgRGBA(255, 255, 255, 255));
        nvgText(tvg, width * 0.5f - tw * 0.5f, 11, "Fullscreen Mode", NULL);

        // draw maximized area
        _maximizedArea->draw(0, 22, width, height-22);
    }else {
        // Draw the areas
        mainLayout->draw(0, 0, width, height);
    }

    if(_show_graph)
        renderGraph(vg, 5,height-40, &fps);

    // Draw all my own data
    if(_external2dDraw!=NULL)
        _external2dDraw(vg);

    // Draw popups if there are one
    drawPopup();

    // File browser
    _fileBrowser->draw();

    nvgEndFrame((NVGcontext*)vg);

    if(_maximizedArea!=NULL){
        // 3D draw
        _maximizedArea->draw3d(0, 22, width, height-22);

    }else if( _popup_type == NO_POPUP) {
        mainLayout->draw3d(0, 0, width, height);
    }



    // Swap buffers
    glfwSwapBuffers((GLFWwindow*)glfw_window);
}

void OGUIWindow::setExternal2DDraw(std::function<void(void* context)> func) {
    _external2dDraw= func;
}

void OGUIWindow::setCursor(int cursor_type)
{
    if(cursor_type!=0)
        actual_cursor_type=cursor_type;
}

void OGUIWindow::drawCursor()
{
    glfwSetCursor((GLFWwindow*)glfw_window, (GLFWcursor*)mouse_cursors_.at(actual_cursor_type));
}

OGUILayout* OGUIWindow::getMainLayout(){
    return mainLayout;
}

OGUIWidget* OGUIWindow::getKeyFocusWidget()
{
    return key_focus_widget_;
}

void OGUIWindow::setKeyFocus(OGUIWidget* widget)
{
    key_focus_widget_=widget;
}

int OGUIWindow::getWindowHeight() {
    return height;
}

int OGUIWindow::getWindowWidth() {
    return width;
}

void OGUIWindow::close(){
    // ToDo improve returns an Error GLXBadDrawable
    glfwDestroyWindow((GLFWwindow*)glfw_window);
}

int OGUIWindow::popup(string title, string text, int  type) {
    _popup_title= title;
    _popup_type= type;
    _popup_text= text;
    _popup_result= -1;
    if(_window_status!=0) {
        while (_popup_result == -1) {
            usleep(1000);// Wait a milisec
        }
    }
    return _popup_result;
}

void OGUIWindow::drawPopup(){
    if(_popup_type != NO_POPUP) {
        NVGcontext *tmp_vg = (NVGcontext *) vg;
        nvgBeginPath(tmp_vg);
        nvgRect(tmp_vg, 0, 0, width, height);
        nvgFillColor(tmp_vg, nvgRGBA(28,30,34, 220));
        nvgFill(tmp_vg);

        int x= (width/2)-200;
        int y= (height/2)-75;

        // Draw box
        nvgBeginPath(tmp_vg);
        nvgRoundedRect(tmp_vg, x, y, 400, 180, 5);
        nvgFillColor(tmp_vg, nvgRGBA(0, 0, 0, 255));
        nvgFill(tmp_vg);

        // Draw icon
        nvgFontSize(tmp_vg, 18.0f);
        nvgFontFace(tmp_vg, "icons");
        nvgTextAlign(tmp_vg,NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
        if(_popup_type==POPUP_ALERT) {
            nvgFillColor(tmp_vg, nvgRGBA(255,200,0,255));
            nvgText(tmp_vg, x + 10, y + 10, "\uF071", NULL);
        }else if(_popup_type==POPUP_ERROR) {
            nvgFillColor(tmp_vg, nvgRGBA(255,84,0,255));
            nvgText(tmp_vg, x + 10, y + 10, "\uF057", NULL);
        }else if(_popup_type==POPUP_CONFIRM) {
            nvgFillColor(tmp_vg, nvgRGBA(0,84,255,255));
            nvgText(tmp_vg, x + 10, y + 10, "\uF05a", NULL);
        }

        // Draw title
        nvgFontSize(tmp_vg, 18.0f);
        nvgFontFace(tmp_vg, "sans-bold");
        nvgTextAlign(tmp_vg,NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
        nvgFillColor(tmp_vg, nvgRGBA(255,255,255,255));
        nvgText(tmp_vg, x+30,y+10,_popup_title.c_str(), NULL);

        // Draw text
        drawParagraph(tmp_vg, _popup_text.c_str(), x+10, y+40, 380, 100, mouse_x, mouse_y);

        // Draw buttons
        if(drawBasicButton(tmp_vg, this, "Accept", x+400-100, y+180-30, 90, 20, mouse_x, mouse_y)){
            _popup_result=1;
            _popup_type= NO_POPUP;
        }

        if(_popup_type==POPUP_CONFIRM) {
            if (drawBasicButton(tmp_vg, this, "Cancel", x + 400 - 200, y + 180 - 30, 90, 20, mouse_x, mouse_y)) {
                _popup_result = 0;
                _popup_type = NO_POPUP;
            }
        }

        // End button


    }

}


int OGUIWindow::getStatus() {
    return _window_status;
}

string OGUIWindow::openFileBrowser(string path, std::vector<string> filter) {
    return _fileBrowser->open(path, filter);
}

void OGUIWindow::maximizeArea(OGUIArea *area) {
    _maximizedArea= area;
}
} /* End of namespace OpenCVGUI */
