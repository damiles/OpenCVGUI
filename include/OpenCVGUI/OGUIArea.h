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
    int x, y, width, height;
    int r,g,b;
    std::string title;
 public:

    OGUIWindow *window;

    /**
     * @element-type OGUILayout
     */
    OGUILayout *layout;
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIArea_h
