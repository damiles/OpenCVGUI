#ifndef OpenCVGUI_OGUIImageArea_h
#define OpenCVGUI_OGUIImageArea_h

#include "OGUIArea.h"


namespace OpenCVGUI {

class OGUIImageArea : public OGUIArea {

 public:

    virtual void draw();

    OGUIImageArea(OGUIWindow* window);
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIImageArea_h
