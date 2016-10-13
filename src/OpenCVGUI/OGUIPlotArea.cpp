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
        if(!data_lock) {
            is_drawing= true;
            drawPlot();
            is_drawing= false;
        }

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
        is_drawing= false;
        data_lock= true;
        ((Mat*)d)->copyTo(data);
        data= data.reshape(0,1);
        data_lock= false;
    }

    void OGUIPlotArea::replot(void *d) {
        while(is_drawing){
//            cout << "wait" << endl;
        }
        data_lock= true;
        ((Mat*)d)->copyTo(data);
        data= data.reshape(0,1);
        data_lock= false;
    }

    void OGUIPlotArea::drawPlot() {
        float margin= 120;
        float start_x= x+margin/2.0f;
        float x_length= data.cols;
        float dx= (width-margin)/x_length;

        double max_value, min_value;
        minMaxLoc(data, &min_value, &max_value, NULL, NULL);
        float start_y= y+height-margin/2.0f;
        float dy= (height-margin);
        //float m= (max_value-min_value);
        float m= (max_value);

        NVGcontext* vg= (NVGcontext*)(window->vg);
        nvgBeginPath(vg);
        nvgMoveTo(vg, start_x, start_y);

        for (int i = 0; i < data.cols; i++) {
            nvgLineTo(vg, start_x + i * dx, start_y - dy * ((data.at<float>(i) ) / m));
        }
        nvgStrokeColor(vg, nvgRGBA(0,160,192,255));
        nvgStrokeWidth(vg, 2.0f);
        nvgStroke(vg);

        // Dots
        for (int i = 0; i < data.cols; i++) {
            nvgBeginPath(vg);
            nvgCircle(vg, start_x + i * dx, start_y - dy * ((data.at<float>(i) ) / m), 4.0f);
            nvgFillColor(vg, nvgRGBA(0, 160, 192, 255));
            nvgFill(vg);

            nvgBeginPath(vg);
            nvgCircle(vg, start_x + i * dx, start_y - dy * ((data.at<float>(i) ) / m), 2.0f);
            nvgFillColor(vg, nvgRGBA(220,220,220, 255));
            nvgFill(vg);
        }

        // X axis
        nvgBeginPath(vg);
        nvgMoveTo(vg, start_x, start_y);
        nvgLineTo(vg, start_x+width-margin, start_y);
        nvgStrokeColor(vg, nvgRGBA(255,255,255,255));
        nvgStrokeWidth(vg, 1.0f);
        nvgStroke(vg);

        // Lines x axis coord
        for(int i=1; i<=10; i++){
            nvgBeginPath(vg);
            nvgMoveTo(vg, start_x+i*dx*10, start_y);
            nvgLineTo(vg, start_x+i*dx*10, start_y+5);
            nvgStrokeColor(vg, nvgRGBA(255,255,255,255));
            nvgStrokeWidth(vg, 1.0f);
            nvgStroke(vg);

            // Text label
            stringstream ss;
            ss << i*10;
            nvgFontSize(vg, 12.0f);
            nvgFontFace(vg, "sans");
            float tw = nvgTextBounds(vg, 0, 0, ss.str().c_str(), NULL, NULL);
            nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
            nvgText(vg, start_x+i*dx*10, start_y+10, ss.str().c_str(), NULL);
        }

        // Y axis
        nvgBeginPath(vg);
        nvgMoveTo(vg, start_x, start_y);
        nvgLineTo(vg, start_x, start_y-dy);
        nvgStrokeColor(vg, nvgRGBA(255,255,255,255));
        nvgStrokeWidth(vg, 1.0f);
        nvgStroke(vg);

        // Lines x axis coord
        for(int i=1; i<=10; i++){
            nvgBeginPath(vg);
            nvgMoveTo(vg, start_x, start_y-(i/10.0f)*dy);
            nvgLineTo(vg, start_x-5, start_y-(i/10.0f)*dy);
            nvgStrokeColor(vg, nvgRGBA(255,255,255,255));
            nvgStrokeWidth(vg, 1.0f);
            nvgStroke(vg);

            // Text label
            stringstream ss;
            ss.precision(2);
            ss << (i/10.0f)*m;
            nvgFontSize(vg, 12.0f);
            nvgFontFace(vg, "sans");
            float tw = nvgTextBounds(vg, 0, 0, ss.str().c_str(), NULL, NULL);
            nvgTextAlign(vg, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
            nvgText(vg, start_x-10, start_y-(i/10.0f)*dy, ss.str().c_str(), NULL);
        }


    }

} /* End of namespace OpenCVGUI */
