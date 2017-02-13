#ifndef OpenCVGUI_OGUIArea_h
#define OpenCVGUI_OGUIArea_h

#include <string>

namespace OpenCVGUI {

    class OGUILayout;
    class OGUIWindow;
    
class OGUIArea {

 public:
    
    OGUIArea(OGUIWindow* window);
    
    virtual void draw(int x, int y, int width, int height);
    virtual void draw3d(int x, int y, int width, int height);
    virtual void updateScrollStatus(double xoffset,double yoffset);
    void drawTitle();
    bool isMouseIn();
    int x, y, width, height;
    int r,g,b;
    std::string title;
    int type=0; // 0 2d type, 1 3d type
    bool isMaximized;
    bool _show_title_bar= true;

 public:

    OGUIWindow *window;

    /**
     * @element-type OGUILayout
     */
    OGUILayout *layout;

private:
    int _count_delay;
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIArea_h
