#ifndef OpenCVGUI_OGUIPlotArea_h
#define OpenCVGUI_OGUIPlotArea_h

#include "OGUIArea.h"
#include <opencv2/core/core.hpp>

using namespace cv;

namespace OpenCVGUI {

class OGUIPlotArea : public OGUIArea {

 public:

    virtual void draw(int x, int y, int width, int height);

    OGUIPlotArea(OGUIWindow* window, std::string title, void* data, std::vector<std::string> labels, float xstep=1.f);
    void replot(void *data, std::vector<std::string> labels, float xstep=1.f);
    std::vector<int> color_scheme={
            0,160,192,
            255,51,102,
            51,255,102,
            204,51,255,
            51,255,204,
            0,204,156,
            0,138,184,
            245,61,0,
            255,204,51,
            99,176,99
    };
private:
    void drawPlot();
    Mat data;
    bool data_lock, is_drawing;
    std::vector<std::string> _labels;
    float _x_step;
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIPlotArea_h
