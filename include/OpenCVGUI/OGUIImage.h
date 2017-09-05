#ifndef OpenCVGUI_OGUIImage_h
#define OpenCVGUI_OGUIImage_h

#include "common.h"
#include "OGUIWidget.h"


namespace OpenCVGUI {

class CV_OGUI_EXPORTS OGUIImage : public OGUIWidget {

 public:

    virtual void draw();

    OGUIImage();
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIImage_h
