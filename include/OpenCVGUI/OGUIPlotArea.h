#ifndef OpenCVGUI_OGUIPlotArea_h
#define OpenCVGUI_OGUIPlotArea_h

#include "OGUIArea.h"
#include <opencv2/core/core.hpp>

using namespace cv;

namespace OpenCVGUI {

    enum PlotTypes{
        PLOT_SERIES,
        PLOT_LINE,
        PLOT_SCATTER,
        PLOT_BARS
    };

class OGUIPlotArea : public OGUIArea {

 public:

    virtual void draw(int x, int y, int width, int height);

    OGUIPlotArea(OGUIWindow* window, std::string title, void* data, std::vector<std::string> labels, float xstep=1.f, int plot_type=0);
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

    void setYScale(bool auto_scale, double min_value, double max_value);
private:
    void drawLinePlot();
    void drawSeriesPlot();
    void drawScatterPlot();
    void drawBarPlot();
    void drawLegends();
    void drawPlot();
    Mat data;
    bool data_lock, is_drawing;
    std::vector<std::string> _labels;
    float _x_step;
    int _count_delay;
    double _max_value, _min_value;
    bool _auto_scale;
    int _plot_type;
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIPlotArea_h
