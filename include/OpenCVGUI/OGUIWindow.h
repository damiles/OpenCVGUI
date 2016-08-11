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
    class OGUIWidget;

    enum MouseCursor {
        DEFAULT_CURSOR,
        HRESIZE_CURSOR,
        VRESIZE_CURSOR,
        HAND_CURSOR,
        TEXT_CURSOR,
        CROSS_CURSOR,
    };
    
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

    // save pointers to all loaded mouse cursors
    std::vector<GLFWcursor*> mouse_cursors_;
    OGUILayout* getMainLayout();
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    void setKeyFocus(OGUIWidget* widget);
    static void charCallback(GLFWwindow* window, unsigned int key);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    OGUIWidget* getKeyFocusWidget();
private:

    /**
     * @element-type OGUIArea
     */
    OGUILayout* mainLayout;
    int width, height;
    string title;
    OGUIWidget* key_focus_widget_;

    PerfGraph fps;
    double prevt = 0;

};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIWindow_h
