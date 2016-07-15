#include "OGUISlider.h"
#include "OGUIArea.h"
#include "OGUIWindow.h"
#include <sstream>

namespace OpenCVGUI {




void OGUISlider::draw(int x, int y, int w)
{
    OGUIWidget::draw(x, y, w);
    OGUIArea* area= (OGUIArea*)(this->area);
    NVGcontext* vg= (area->window->vg);

    NVGpaint bg, knob;

    // Draw text
    nvgFontSize(vg, 18.0f);
    nvgFontFace(vg, "sans-bold");
    nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
    nvgFillColor(vg, nvgRGBA(0,0,0,255));
    nvgText(vg, x+2,y-1,title, NULL);
    nvgFillColor(vg, nvgRGBA(255,255,255,255));
    nvgText(vg, x+2,y,title, NULL);

    y=y+20;
    // Slot
    bg = nvgBoxGradient(vg, x, y, w,20, 2,2, nvgRGBA(0,0,0,32), nvgRGBA(0,0,0,128));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x, y, w,20, 2);
    nvgFillPaint(vg, bg);
    nvgFill(vg);

    double progress_pos= w*(value/(max-min));
    bg = nvgBoxGradient(vg, x, y, progress_pos,20, 2,2, nvgRGBA(29,157,206,64), nvgRGBA(29,157,206,128));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x, y, progress_pos,20, 2);
    nvgFillPaint(vg, bg);
    nvgFill(vg);


    // Draw text
    stringstream ss;
    ss << value;
    nvgFontSize(vg, 12.0f);
    nvgFontFace(vg, "sans-bold");
    float tw = nvgTextBounds(vg, 0,0, ss.str().c_str(), NULL, NULL);
    nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
    nvgFillColor(vg, nvgRGBA(0,0,0,255));
    nvgText(vg, x+w-tw-5,y+10.0f-1,ss.str().c_str(), NULL);
    nvgFillColor(vg, nvgRGBA(255,255,255,255));
    nvgText(vg, x+w-tw-5,y+10.0f,ss.str().c_str(), NULL);

    if(area->window->mouse_x > x &&
       area->window->mouse_x < x+w &&
       area->window->mouse_y > y &&
       area->window->mouse_y < y + 20
            ) {

        area->window->setCursor(1);

        if (area->window->mouse_state == GLFW_PRESS){
            actual_press_status = GLFW_PRESS;
            if(btn_click_callback!=NULL)
                btn_click_callback();
        }else if(area->window->mouse_state == GLFW_RELEASE && actual_press_status == GLFW_PRESS){
            actual_press_status = GLFW_RELEASE;
        }
    }else {
        area->window->setCursor(0);
    }

    y=y-20;
}

void OGUISlider::setCallBack(std::function<void()> func) {
    btn_click_callback= func;
}

OGUISlider::OGUISlider(const char* title,double min,double max, double value)
{
    this->height= 40;
    this->title= title;
    this->min= min;
    this->max= max;
    this->value= value;
    actual_press_status = GLFW_RELEASE;

}

} /* End of namespace OpenCVGUI */
