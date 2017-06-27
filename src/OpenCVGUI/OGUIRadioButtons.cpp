//
// Created by damiles on 10/08/16.
//

#include "OGUIRadioButtons.h"
#include "OGUIArea.h"
#include "OGUIWindow.h"

#include <GL/glew.h>
#ifdef __APPLE__
#   define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
#include "nanovg.h"

using namespace std;

namespace OpenCVGUI {

    OGUIRadioButtons::OGUIRadioButtons(const char *title, vector <string> options, int default_selected)
    {
        title_= title;
        options_= options;
        height= 20 + options_.size()*20;
        selected_= default_selected;
    }

    void OGUIRadioButtons::draw(int x, int y, int width)
    {
        OGUIWidget::draw(x, y, width);
        OGUIArea* area= (OGUIArea*)(this->area);
        NVGcontext* vg= (NVGcontext*)(area->window->vg);

        NVGpaint bg;

        // Draw text
        nvgFontSize(vg, 18.0f*area->window->font_scale);
        nvgFontFace(vg, "sans-bold");
        nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
        nvgFillColor(vg, nvgRGBA(0,0,0,255));
        nvgText(vg, x+2, y-1, title_, NULL);
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 180));
        nvgText(vg, x+2, y, title_, NULL);

        for(int i=0; i<options_.size(); i++)
        {
            y+=20;
            // Draw circle
            nvgBeginPath(vg);
            nvgCircle(vg, x+10, y+10, 8);
            nvgFillColor(vg, nvgRGBA(50,53,58,255));
            nvgFill(vg);

            if(selected_==i)
            {
                nvgBeginPath(vg);
                nvgCircle(vg, x+10, y+10, 6);
                nvgFillColor(vg, nvgRGBA(40,110,160,255));
                nvgFill(vg);
            }else if(area->window->mouse_x > x &&
                     area->window->mouse_x < x+width &&
                     area->window->mouse_y > y &&
                     area->window->mouse_y < y + 20)
            {
                nvgBeginPath(vg);
                nvgCircle(vg, x+10, y+10, 6);
                nvgFillColor(vg, nvgRGBA(255,255,255,25));
                nvgFill(vg);
                area->window->setCursor(3);
                if (area->window->mouse_state == GLFW_PRESS)
                {
                    // call callback if the option changes
                    if(selected_!=i && on_change_callback!=NULL)
                            on_change_callback(i);
                    selected_=i;
                }

            } else {
                area->window->setCursor(0);
            }


            // Draw text
            nvgFontSize(vg, 18.0f*area->window->font_scale);
            nvgFontFace(vg, "sans");
            nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
            nvgFillColor(vg, nvgRGBA(0,0,0,128));
            nvgText(vg, x+25, y-1, options_.at(i).c_str(), NULL);
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 128));
            nvgText(vg, x+25, y, options_.at(i).c_str(), NULL);
        }

    }

    void OGUIRadioButtons::setCallBack(std::function<void(int)> func) {
        on_change_callback= func;
    }

}