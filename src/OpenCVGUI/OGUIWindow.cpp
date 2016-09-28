#include "OGUIWindow.h"
#include "OGUIArea.h"
#include "OGUILayout.h"
#include "OGUIWidget.h"

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
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

OGUIWindow::OGUIWindow(int width,int height,const char* title, int layoutOrientation)
{
    this->width= width;
	this->height= height;
	this->title.append(title);

    init();
    
    this->mainLayout= new OGUILayout(this, layoutOrientation);
    this->mainLayout->title="Main  Layout";
    this->actual_cursor_type=0;
    this->key_focus_widget_= NULL;

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

int OGUIWindow::init()
{
    
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
    

    if(vg==NULL){
        fprintf(stderr, "Error: can not init nanovg");
        return 0;
    }

    nvgCreateFont((NVGcontext*)vg, "sans", "../resources/Roboto-Regular.ttf");
    nvgCreateFont((NVGcontext*)vg, "sans-bold", "../resources/fonts/Varela_Round/VarelaRound-Regular.ttf");
    nvgCreateFont((NVGcontext*)vg, "icons", "../resources/fonts/fontawesome-webfont.ttf");

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
}

void OGUIWindow::addArea(OGUIArea* area)
{
    mainLayout->addArea(area);
}

bool OGUIWindow::update(){
    glfwMakeContextCurrent((GLFWwindow *)glfw_window);
    if(!glfwWindowShouldClose((GLFWwindow *)glfw_window)) {

        double t, dt;
        t = glfwGetTime();
        dt = t - prevt;
        prevt = t;
        updateGraph(&fps, dt);


        glfwGetCursorPos((GLFWwindow *) glfw_window, &mouse_x, &mouse_y);
        mouse_left_state = glfwGetMouseButton((GLFWwindow *) glfw_window, GLFW_MOUSE_BUTTON_LEFT);
        mouse_state = glfwGetMouseButton((GLFWwindow *) glfw_window, GLFW_MOUSE_BUTTON_LEFT);

        actual_cursor_type = 0;
        draw();
        drawCursor();

        glfwPollEvents();

        return true;
    }else{
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

    // Draw all my own data
    
    // Draw the areas 
    mainLayout->draw(0,0,width, height);
    

    renderGraph(vg, width-205,height-40, &fps);

    nvgEndFrame((NVGcontext*)vg);

    // 3D draw
    mainLayout->draw3d(0,0,width, height);

    // Swap buffers
    glfwSwapBuffers((GLFWwindow*)glfw_window);
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

} /* End of namespace OpenCVGUI */
