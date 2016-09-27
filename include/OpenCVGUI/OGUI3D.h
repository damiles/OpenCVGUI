#ifndef OpenCVGUI_OGUI3D_h
#define OpenCVGUI_OGUI3D_h

#include "OGUIArea.h"
#include <string>

namespace OpenCVGUI {

class OGUI3D : public OGUIArea {

 public:

    virtual void draw(int x, int y, int width, int height);

    OGUI3D(OGUIWindow* window, std::string title);
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUI3D_h
