#ifndef OpenCVGUI_OGUIFormArea_h
#define OpenCVGUI_OGUIFormArea_h

#include <vector>

#include "OGUIArea.h"

namespace OpenCVGUI {
class OGUIWidget;
} /* End of namespace OpenCVGUI */

namespace OpenCVGUI {

class OGUIFormArea : public OGUIArea {

 public:

    virtual void draw(int x, int y, int width, int height);

    OGUIFormArea(OGUIWindow* window);
    void addWidget(OGUIWidget* widget);

 public:

    /**
     * @element-type OGUIWidget
     */
    std::vector< OGUIWidget* > widgets;
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIFormArea_h
