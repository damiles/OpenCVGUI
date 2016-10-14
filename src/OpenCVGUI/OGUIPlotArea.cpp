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

    OGUIPlotArea::OGUIPlotArea(OGUIWindow* window, std::string title, void* d, std::vector<string> labels, float xstep): OGUIArea(window)
    {
        color_scheme={
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
        this->title= title;
        is_drawing= false;
        data_lock= true;
        ((Mat*)d)->copyTo(data);
        //data= data.reshape(0,1);
        _labels= labels;
        _x_step= xstep;
        data_lock= false;

    }

    void OGUIPlotArea::replot(void *d, std::vector<string> labels, float xstep) {
        while(is_drawing){
//            cout << "wait" << endl;
        }
        data_lock= true;
        ((Mat*)d)->copyTo(data);
        //data= data.reshape(0,1);
        _labels= labels;
        _x_step= xstep;
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

        // Plot each graph
        for(int p=0; p<data.rows; p++) {
            int c=p*3;
            NVGcolor color= nvgRGBA(color_scheme.at(c), color_scheme.at(c+1), color_scheme.at(c+2), 255);
            nvgBeginPath(vg);
            nvgMoveTo(vg, start_x, start_y);

            for (int i = 0; i < data.cols; i++) {
                nvgLineTo(vg, start_x + i * dx, start_y - dy * ((data.at<float>(p,i)) / m));
            }
            nvgStrokeColor(vg, color);
            nvgStrokeWidth(vg, 2.0f);
            nvgStroke(vg);

            // Dots
            for (int i = 0; i < data.cols; i++) {
                float dot_x=start_x + i * dx;
                float dot_y=start_y - dy * ((data.at<float>(p,i)) / m);
                nvgBeginPath(vg);
                nvgCircle(vg, dot_x, dot_y, 4.0f);
                nvgFillColor(vg, color);
                nvgFill(vg);

                nvgBeginPath(vg);
                nvgCircle(vg, dot_x, dot_y, 2.0f);
                nvgFillColor(vg, nvgRGBA(220, 220, 220, 255));
                nvgFill(vg);

            }
        }

        nvgResetScissor(vg);
        // Plot hover labels
        for(int p=0; p<data.rows; p++) {
            int c=p*3;
            NVGcolor color= nvgRGBA(color_scheme.at(c), color_scheme.at(c+1), color_scheme.at(c+2), 255);
            // Dots
            for (int i = 0; i < data.cols; i++) {
                float dot_x=start_x + i * dx;
                float dot_y=start_y - dy * ((data.at<float>(p,i)) / m);
                // Draw hover label
                if(isMouseIn()) {
                    if( dot_y-4<= window->mouse_y && window->mouse_y <= dot_y+4 &&
                        dot_x-4<= window->mouse_x && window->mouse_x <= dot_x+4 ){

                        // Calculate text width for box
                        nvgFontSize(vg, 16.0f);
                        nvgFontFace(vg, "sans");
                        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                        stringstream ss_value;
                        ss_value.precision(4);
                        ss_value << _labels.at(p) <<": "<< (i*_x_step) << ", " <<data.at<float>(p,i);
                        float tw = nvgTextBounds(vg, 0, 0, ss_value.str().c_str(), NULL, NULL);

                        nvgBeginPath(vg);
                        nvgRect(vg, dot_x-tw-20, dot_y-32, tw+20, 32);
                        nvgFillColor(vg, nvgRGBA(0, 0, 0, 200));
                        nvgFill(vg);

                        // Draw text
                        nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
                        nvgText(vg, dot_x - tw - 10, dot_y - 16, ss_value.str().c_str(), NULL);
                    }
                }
            }
        }
        nvgScissor(vg, x, y, width, height);

        ///Plot labels box
        nvgBeginPath(vg);
        nvgRect(vg, x + width - 100-20, y + 40, 120, 15 + data.rows*20);
        nvgFillColor(vg, nvgRGBA(0, 0, 0, 100));
        nvgFill(vg);
        for(int p=0; p<data.rows; p++) {
            int c = p * 3;
            NVGcolor color = nvgRGBA(color_scheme.at(c), color_scheme.at(c + 1), color_scheme.at(c + 2), 255);
            float dot_x=x + width - 100;
            float dot_y=y + 60 + p*20;
            nvgBeginPath(vg);
            nvgCircle(vg, dot_x, dot_y, 4.0f);
            nvgFillColor(vg, color);
            nvgFill(vg);

            // Calculate text width for box
            nvgFontSize(vg, 16.0f);
            nvgFontFace(vg, "sans");
            nvgTextAlign(vg, NVG_ALIGN_LEFT);
            stringstream ss_value;
            ss_value << _labels.at(p);
            // Draw text
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
            nvgText(vg, dot_x + 10, dot_y+3, ss_value.str().c_str(), NULL);
        }

        // X axis
        nvgBeginPath(vg);
        nvgMoveTo(vg, start_x, start_y);
        nvgLineTo(vg, start_x+width-margin, start_y);
        nvgStrokeColor(vg, nvgRGBA(255,255,255,255));
        nvgStrokeWidth(vg, 1.0f);
        nvgStroke(vg);

        // Lines x axis coord
        for(int i=1; i<data.cols; i++){
            int s=5;
            if((i%s)==0) {
                nvgBeginPath(vg);
                nvgMoveTo(vg, start_x + i * dx, start_y);
                nvgLineTo(vg, start_x + i * dx, start_y + 5);
                nvgStrokeColor(vg, nvgRGBA(255, 255, 255, 255));
                nvgStrokeWidth(vg, 1.0f);
                nvgStroke(vg);

                // Text label
                stringstream ss;
                ss << i * _x_step;
                nvgFontSize(vg, 12.0f);
                nvgFontFace(vg, "sans");
                float tw = nvgTextBounds(vg, 0, 0, ss.str().c_str(), NULL, NULL);
                nvgTextAlign(vg, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
                nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
                nvgText(vg, start_x + i * dx, start_y + 10, ss.str().c_str(), NULL);
            }
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
