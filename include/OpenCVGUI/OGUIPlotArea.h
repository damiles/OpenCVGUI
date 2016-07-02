#ifndef OpenCVGUI_OGUIPlotArea_h
#define OpenCVGUI_OGUIPlotArea_h

#include "OGUIArea.h"


namespace OpenCVGUI {

class OGUIPlotArea : public OGUIArea {

 public:

    virtual void draw();

    OGUIPlotArea(OGUIWindow* window);
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIPlotArea_h
