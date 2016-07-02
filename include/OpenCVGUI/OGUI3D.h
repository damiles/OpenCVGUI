#ifndef OpenCVGUI_OGUI3D_h
#define OpenCVGUI_OGUI3D_h

#include "OGUIArea.h"


namespace OpenCVGUI {

class OGUI3D : public OGUIArea {

 public:

    virtual void draw();

    OGUI3D(OGUIWindow* window);
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUI3D_h
