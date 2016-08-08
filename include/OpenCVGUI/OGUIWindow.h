#ifndef OpenCVGUI_OGUIWindow_h
#define OpenCVGUI_OGUIWindow_h

#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include <GL/glew.h>
#ifdef __APPLE__
#   define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
#include "nanovg.h"
#include "perf.h"


using namespace std;

namespace OpenCVGUI {
    int init();
    class OGUILayout;
    class OGUIArea;
    
class OGUIWindow {

 public:

    OGUIWindow(int width,int height,const char* title, int layoutOrientation=0);
    ~OGUIWindow();
    void draw();
    void update();

    GLFWwindow* glfw_window;
    static bool isInitGlfw;
    int init();
    NVGcontext* vg;
    void addArea(OGUIArea* area);
    double mouse_x, mouse_y;
    int mouse_state;
    
    void setCursor(int cursor_type);
    void drawCursor();
    int actual_cursor_type;
    int mouse_left_state;
    GLFWcursor* cursor_hresize;
    GLFWcursor* cursor_vresize;
    GLFWcursor* cursor_hand;
    OGUILayout* getMainLayout();
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
 private:

    /**
     * @element-type OGUIArea
     */
    OGUILayout* mainLayout;
    int width, height;
    string title;

    PerfGraph fps;
    double prevt = 0;

};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIWindow_h
