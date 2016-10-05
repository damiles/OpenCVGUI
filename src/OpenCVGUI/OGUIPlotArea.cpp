#include "OGUIPlotArea.h"
#include "OGUIWindow.h"
#include "nanovg.h"

namespace OpenCVGUI {




    void OGUIPlotArea::draw(int x, int y, int width, int height)
    {
        this->x=x;
        this->y=y;
        this->width= width;
        this->height= height;
        NVGcontext* vg= (NVGcontext*)(window->vg);

        nvgScissor(vg, x, y, width, height);
        OGUIArea::draw(x,y,width,height);

        drawAxis();
        drawPlot();

        if(isMouseIn()) {
            nvgBeginPath(vg);
            nvgRect(vg, x, y, width, 22);
            nvgFillColor(vg, nvgRGBA(0, 0, 0, 100));
            nvgFill(vg);

            // Draw text
            nvgFontSize(vg, 16.0f);
            nvgFontFace(vg, "sans");
            float tw = nvgTextBounds(vg, 0, 0, title.c_str(), NULL, NULL);
            nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
            nvgText(vg, x + width * 0.5f - tw * 0.5f, y + 11, title.c_str(), NULL);
        }
        nvgResetScissor(vg);
    }

    OGUIPlotArea::OGUIPlotArea(OGUIWindow* window, std::string title, void* d): OGUIArea(window)
    {
        this->title= title;
        ((Mat*)d)->copyTo(data);
    }

    void OGUIPlotArea::drawAxis() {

    }

    void OGUIPlotArea::drawPlot() {
        float margin= 40;
        float start_x= x+margin/2.0f;
        float x_length= data.cols;
        float dx= (width-margin)/x_length;

        double max_value, min_value;
        minMaxLoc(data, &min_value, &max_value, NULL, NULL);
        float start_y= y+height-margin/2.0f;
        float dy= (height-margin);
        float m= (max_value-min_value);

        NVGcontext* vg= (NVGcontext*)(window->vg);
        nvgBeginPath(vg);
        nvgMoveTo(vg, start_x, start_y);

        for (int i = 0; i < data.cols; i++)
            nvgLineTo(vg, start_x+i*dx, start_y- dy*((data.at<float>(i)-min_value)/m) );
        nvgStrokeColor(vg, nvgRGBA(0,160,192,255));
        nvgStrokeWidth(vg, 1.0f);
        nvgStroke(vg);

    }

} /* End of namespace OpenCVGUI */
