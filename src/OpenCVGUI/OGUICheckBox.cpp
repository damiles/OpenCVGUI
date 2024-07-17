#include "OGUICheckBox.h"
#include "OGUIWidget.h"
#include "OGUIArea.h"
#include "OGUIWindow.h"
#include <sstream>
#include <GL/glew.h>
#ifdef __APPLE__
#   define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
#include "nanovg.h"

namespace OpenCVGUI {




void OGUICheckBox::draw(int x, int y, int width)
{
    OGUIWidget::draw(x, y, width);
    OGUIArea* area= (OGUIArea*)(this->area);
    NVGcontext* vg= (NVGcontext*)(area->window->vg);

    // Draw text
    nvgFontSize(vg, 18.0f*area->window->font_scale);
    nvgFontFace(vg, "sans-bold");
    nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
    nvgFillColor(vg, nvgRGBA(0,0,0,255));
    nvgText(vg, x+18,y+5,title, NULL);
    nvgFillColor(vg, nvgRGBA(255,255,255,255));
    nvgText(vg, x+18,y+6,title, NULL);

    // Draw box square
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+1,y+9, 12,12, 3);

    if(area->window->mouse_x > x &&
        area->window->mouse_x < x+width &&
        area->window->mouse_y > y &&
        area->window->mouse_y < y + height
        ) {

            nvgFillColor(vg, nvgRGBA(50,53,58,255));
            area->window->setCursor(HAND_CURSOR);
            if (area->window->mouse_state == GLFW_PRESS && actual_press_status != GLFW_PRESS){
                actual_press_status = GLFW_PRESS;
            }else if(area->window->mouse_state == GLFW_RELEASE && actual_press_status == GLFW_PRESS){
                actual_press_status = GLFW_RELEASE;
                value = !value;
                if(btn_click_callback!=NULL)
                    btn_click_callback(value);
            }
    }else {
        nvgFillColor(vg, nvgRGBA(50,53,58,180));
        area->window->setCursor(DEFAULT_CURSOR);
    }

    nvgFill(vg);

    // Draw check square
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+4,y+12, 6,6, 3);

    if (this->value){
        nvgFillColor(vg, nvgRGBA(23, 145, 207,255));
    }else{
        nvgFillColor(vg, nvgRGBA(0, 0, 0,50));
    }
    nvgFill(vg);
}

void OGUICheckBox::setCallBack(std::function<void(bool)> func) {
        btn_click_callback= func;
}

OGUICheckBox::OGUICheckBox(const char* title, bool value)
{
    this->title = title;
    this->value = value;
    this->height = 30;
}

} /* End of namespace OpenCVGUI */
