#include "OGUIWindow.h"
#include "OGUIArea.h"
#include "OGUILayout.h"

#define NANOVG_GL3_IMPLEMENTATION
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
    
    // GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll consume it here.
	glGetError();
    glfwSetErrorCallback(errorcb);
    
  } ;
    
bool OGUIWindow::isInitGlfw= false;

OGUIWindow::OGUIWindow(int width,int height,const char* title, int layoutOrientation)
{
    this->width= width;
	this->height= height;
	this->title.append(title);
    
    this->mainLayout= new OGUILayout(this, layoutOrientation);
    this->mainLayout->title="Main  Layout";
    this->actual_cursor_type=0;
	// Start thread
	window_thread= new thread(&OGUIWindow::window_thread_func, this);

}

OGUIWindow::~OGUIWindow()
{
	is_thread_running= false;
	if(window_thread->joinable()) window_thread->join();
	glfwDestroyWindow(glfw_window);
}

int OGUIWindow::init()
{
    
	this->glfw_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);	
    if (!glfw_window) {
        fprintf(stderr, "Error creating glfw window\n");
        glfwTerminate();
        return 0;
	}

	glfwMakeContextCurrent(glfw_window);
    glfwSwapInterval(0);
    
    // Init GLew
    GLenum err= glewInit();
    if(err != GLEW_OK) {
        printf("Could not init glew.\n");
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 0;
    }
    
    vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
    if(vg==NULL){
        fprintf(stderr, "Error: can not init nanovg");
        return 0;
    }
    
    cursor_hresize = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    cursor_vresize = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    glfwSetInputMode(glfw_window, GLFW_STICKY_MOUSE_BUTTONS, 1);
    
    return 1;
}

void OGUIWindow::addArea(OGUIArea* area)
{
    mainLayout->addArea(area);
}

void OGUIWindow::window_thread_func()
{
    
    if(!init())
        return;
    
    std::cout << "Running thread of window " << title << std::endl;
    
	while(is_thread_running)
	{
		
        glfwGetCursorPos(glfw_window, &mouse_x, &mouse_y);
        mouse_left_state = glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_LEFT);
        
        actual_cursor_type=0;
        draw();
		drawCursor();
        
		glfwPollEvents();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        
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
    glClear(GL_COLOR_BUFFER_BIT);

    nvgBeginFrame(vg, width, height, ratio);

    // Draw all my own data
    
    // Draw the areas 
    mainLayout->draw(0,0,width, height);
    
    nvgEndFrame(vg);
    // Swap buffers
    glfwSwapBuffers(glfw_window);
}

void OGUIWindow::setCursor(int cursor_type)
{
    if(cursor_type!=0)
        actual_cursor_type=cursor_type;
}

void OGUIWindow::drawCursor()
{
    if(actual_cursor_type==0)
        glfwSetCursor(glfw_window, NULL);
    else if(actual_cursor_type==1)
        glfwSetCursor(glfw_window, cursor_hresize);
    else if(actual_cursor_type==2)
        glfwSetCursor(glfw_window, cursor_vresize);
    
}

OGUILayout* OGUIWindow::getMainLayout(){
    return mainLayout;
}

} /* End of namespace OpenCVGUI */
