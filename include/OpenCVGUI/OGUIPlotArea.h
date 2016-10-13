#ifndef OpenCVGUI_OGUIPlotArea_h
#define OpenCVGUI_OGUIPlotArea_h

#include "OGUIArea.h"
#include <opencv2/core/core.hpp>

using namespace cv;

namespace OpenCVGUI {

class OGUIPlotArea : public OGUIArea {

 public:

    virtual void draw(int x, int y, int width, int height);

    OGUIPlotArea(OGUIWindow* window, std::string title, void* data);
    void replot(void *data);
private:
    void drawPlot();
    Mat data;
    bool data_lock, is_drawing;
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIPlotArea_h
