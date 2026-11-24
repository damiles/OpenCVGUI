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

        return 1;
      } ;
    
bool OGUIWindow::isInitGlfw= false;

OGUIWindow::OGUIWindow(int width, int height,
                       const char* title,
                       int layoutOrientation,
                       bool fullScreen)
{
    this->_ui_thread_id= std::this_thread::get_id();
    this->_window_status= 1;
    this->width= width;
	this->height= height;
	this->title.append(title);
    this->font_scale=1.0;

    init(fullScreen);
    
    this->mainLayout= new OGUILayout(this, layoutOrientation);
    this->mainLayout->title="Main  Layout";
    this->actual_cursor_type=0;
    this->key_focus_widget_= NULL;
    this->key_pressed= 0;
    this->_show_graph= false;
    this->_fileBrowser= new OGUIFileBrowser(this);
    this->_maximizedArea= NULL;
}

OGUIWindow::~OGUIWindow()
{
#ifdef OGUI_GL3
    nvgDeleteGL3(vg);
#else
    nvgDeleteGL2(vg);
#endif
	
    glfwDestroyWindow(glfw_window);
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

    glfwSetWindowUserPointer(glfw_window, this);
	glfwMakeContextCurrent(glfw_window);
    glfwSwapInterval(1); // Enable vsync to avoid drawing more frames than the screen shows
    
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

    nvgCreateFontMem(vg, "sans", font_roboto, font_roboto_size, 1);
    nvgCreateFontMem(vg, "sans-bold", font_valera, font_valera_size, 1);
    nvgCreateFontMem(vg, "icons", font_awesome, font_awesome_size, 1);

    // Load all cursors
    mouse_cursors_.push_back(glfwCreateStandardCursor(GLFW_ARROW_CURSOR));
    mouse_cursors_.push_back(glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR));
    mouse_cursors_.push_back(glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR));
    mouse_cursors_.push_back(glfwCreateStandardCursor(GLFW_HAND_CURSOR));
    mouse_cursors_.push_back(glfwCreateStandardCursor(GLFW_IBEAM_CURSOR));
    mouse_cursors_.push_back(glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR));

    glfwSetInputMode(glfw_window, GLFW_STICKY_MOUSE_BUTTONS, 1);

    glfwSetScrollCallback(glfw_window, OGUIWindow::scroll_callback);
    glfwSetCharCallback(glfw_window, OGUIWindow::charCallback);
    glfwSetKeyCallback(glfw_window, OGUIWindow::keyCallback);

    glfwSetTime(0);
    prevt = glfwGetTime();

    return 1;
}

void OGUIWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    OGUIWindow* oguiwindow= (OGUIWindow*)glfwGetWindowUserPointer(window);
    oguiwindow->mainLayout->updateScrollStatus(xoffset, yoffset);
}

void OGUIWindow::charCallback(GLFWwindow* window, unsigned int key)
{
    OGUIWindow* oguiwindow= (OGUIWindow*)glfwGetWindowUserPointer(window);
    OGUIWidget* widget= oguiwindow->getKeyFocusWidget();
    if(widget!=NULL){
        widget->characterCallback(key);
    }
}

void OGUIWindow::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    OGUIWindow* oguiwindow= (OGUIWindow*)glfwGetWindowUserPointer(window);
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
    glfwMakeContextCurrent(glfw_window);
    if(!glfwWindowShouldClose(glfw_window)) {

        runUITasks();

        glfwGetCursorPos(glfw_window, &mouse_x, &mouse_y);
        mouse_left_state = glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_LEFT);
        mouse_state = glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_LEFT);

        actual_cursor_type = 0;
        draw();
        drawCursor();

        glfwPollEvents();

        return true;
    }else{
        {
            // Release threads waiting for a popup
            std::lock_guard<std::mutex> lock(_state_mutex);
            _popup_result= 0;
            _popup_type= NO_POPUP;
            _window_status= 0;
        }
        _popup_closed.notify_all();
        _fileBrowser->cancel();
        return false;
    }
}

void OGUIWindow::draw()
{
    float ratio;
    
    // Get size and clean buffer
    glfwGetFramebufferSize(glfw_window, &width, &height);
    ratio = width / (float) height;
    
    glViewport(0, 0, width, height);
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame(vg, width, height, ratio);

    if(_maximizedArea!=NULL){
        // Draw title
        NVGcontext* tvg= vg;
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
    bool popup_visible= drawPopup();

    // File browser
    _fileBrowser->draw();

    std::unique_lock<std::mutex> state_lock(_state_mutex);
    if (_show_progress){
        NVGcontext* tvg= vg;
        nvgResetScissor(tvg);
        
        nvgBeginPath(tvg);
        nvgRect(tvg, 0, 0, width, height);
        nvgFillColor(tvg, nvgRGBA(0, 0, 0, 220));
        nvgFill(tvg);
        
        nvgBeginPath(tvg);
        nvgRect(tvg, 0, 0, _progress*width, 14);
        nvgFillColor(tvg, nvgRGBA(100, 0, 0, 255));
        nvgFill(tvg);
        drawLabel(tvg, _progress_title.c_str(), 5, 7, "sans", 14.0f, 255,255,255,255 );
    }
    state_lock.unlock();
    
    nvgEndFrame(vg);

    if(_maximizedArea!=NULL){
        // 3D draw
        _maximizedArea->draw3d(0, 22, width, height-22);

    }else if(!popup_visible) {
        mainLayout->draw3d(0, 0, width, height);
    }



    // Swap buffers
    glfwSwapBuffers(glfw_window);
}

void OGUIWindow::show_progress(string title, float percentage) {
    std::lock_guard<std::mutex> lock(_state_mutex);
    _progress= percentage;
    _show_progress=true;
    _progress_title= title;
}

void OGUIWindow::runOnUIThread(std::function<void()> func) {
    std::lock_guard<std::mutex> lock(_ui_tasks_mutex);
    _ui_tasks.push_back(func);
}

void OGUIWindow::runUITasks() {
    std::vector<std::function<void()>> tasks;
    {
        std::lock_guard<std::mutex> lock(_ui_tasks_mutex);
        tasks.swap(_ui_tasks);
    }
    for(size_t i=0; i<tasks.size(); i++)
        tasks[i]();
}

void OGUIWindow::hide_progress(){
    std::lock_guard<std::mutex> lock(_state_mutex);
    _show_progress=false;
}

void OGUIWindow::setExternal2DDraw(std::function<void(NVGcontext* context)> func) {
    _external2dDraw= func;
}

void OGUIWindow::setCursor(int cursor_type)
{
    if(cursor_type!=0)
        actual_cursor_type=cursor_type;
}

void OGUIWindow::drawCursor()
{
    glfwSetCursor(glfw_window, mouse_cursors_.at(actual_cursor_type));
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
    glfwDestroyWindow(glfw_window);
}

bool OGUIWindow::isUIThread() {
    return std::this_thread::get_id() == _ui_thread_id;
}

int OGUIWindow::popup(string title, string text, int  type) {
    // The UI thread draws the popup, waiting here would block it forever
    if(isUIThread()) {
        cerr << "OpenCVGUI: popup can not be called from the UI thread" << endl;
        return 0;
    }

    std::unique_lock<std::mutex> lock(_state_mutex);
    if(_window_status==0)
        return 0;
    _popup_title= title;
    _popup_type= type;
    _popup_text= text;
    _popup_result= -1;
    _popup_closed.wait(lock, [this]() { return _popup_result != -1; });
    return _popup_result;
}

bool OGUIWindow::drawPopup(){
    std::unique_lock<std::mutex> lock(_state_mutex);
    if(_popup_type != NO_POPUP) {
        NVGcontext *tmp_vg = vg;
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

        if(_popup_type == NO_POPUP) {
            lock.unlock();
            _popup_closed.notify_all();
        }
        return true;
    }
    return false;
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
