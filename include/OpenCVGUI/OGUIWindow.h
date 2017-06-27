#ifndef OpenCVGUI_OGUIWindow_h
#define OpenCVGUI_OGUIWindow_h

#include <string>
#include <vector>
#include <thread>
#include <iostream>
#include <unistd.h>
#include "perf.h"

#include "OGUI3D.h"
#include "OGUIFileBrowser.h"

using namespace std;

// Font images
extern unsigned char font_roboto[];
extern int font_roboto_size;
extern int font_awesome_size;
extern unsigned char font_awesome[];
extern int font_valera_size;
extern unsigned char font_valera[];


namespace OpenCVGUI {
    int init();
    class OGUILayout;
    class OGUIArea;
    class OGUIWidget;
    class OGUIFileBrowser;

    enum MouseCursor {
        DEFAULT_CURSOR,
        HRESIZE_CURSOR,
        VRESIZE_CURSOR,
        HAND_CURSOR,
        TEXT_CURSOR,
        CROSS_CURSOR,
    };

    enum PopupTypes{
        NO_POPUP,
        POPUP_CONFIRM,
        POPUP_ALERT,
        POPUP_ERROR
    };


class OGUIWindow {

 public:

    OGUIWindow(int width,int height,const char* title, int layoutOrientation=0, bool fullScreen= false);
    ~OGUIWindow();
    void draw();
    bool update(); // Return false if window is closed

    void* glfw_window;//GLFWwindow
    static bool isInitGlfw;
    int init(bool fullScreen);
    void* vg;//NVGcontext
    void addArea(OGUIArea* area);
    double mouse_x, mouse_y;
    int mouse_state;
    int key_pressed;
    
    void setCursor(int cursor_type);
    void drawCursor();
    int actual_cursor_type;
    int mouse_left_state;

    // save pointers to all loaded mouse cursors
    std::vector<void*> mouse_cursors_;//GLFWcursor
    OGUILayout* getMainLayout();
    static void scroll_callback(void* window, double xoffset, double yoffset);//GLFWwindow

    void setKeyFocus(OGUIWidget* widget);
    static void charCallback(void* window, unsigned int key);//GLFWwindow
    static void keyCallback(void* window, int key, int scancode, int action, int mods);//GLFWwindow
    OGUIWidget* getKeyFocusWidget();

    int getWindowHeight();
    int getWindowWidth();
    void updatePerfGraph();
    void showPerfGraph(bool show);
    void setExternal2DDraw(std::function<void(void* context)> func);

    /// Popups
    int popup(string title, string text, int type=2);

    /// File browser
    string openFileBrowser(string path, std::vector<string> filter);
    //string saveFileBrowser(string path='.');

    // Force to close window
    void close();

    // Get window status 0 is close 1 is open
    int getStatus();

    // Set Area as MaximizedArea
    void maximizeArea(OGUIArea *area);

    // font scale, default value 1.0
    float font_scale=1.0;

private:

    /**
     * @element-type OGUIArea
     */
    OGUILayout* mainLayout;
    int width, height;
    string title;
    OGUIWidget* key_focus_widget_;
    int _window_status= 1; // 0 close, 1 open

    PerfGraph fps;
    double prevt = 0;
    bool _show_graph= false;

    std::function<void(void* context)> _external2dDraw;

    // Popups
    void drawPopup();
    int _popup_type=0; // 0 no popup visible, 1 confirm, 2 alert, 3 error
    string _popup_text;
    string _popup_title;
    int _popup_result=0;

    // File Browser
    OGUIFileBrowser *_fileBrowser;

    // Maxmized Area
    OGUIArea *_maximizedArea;

};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIWindow_h
