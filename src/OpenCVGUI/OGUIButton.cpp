#include "OGUIButton.h"
#include "OGUIWidget.h"
#include "OGUIArea.h"
#include "OGUIWindow.h"

namespace OpenCVGUI {

    void OGUIButton::draw(int x, int y, int width, int height)
    {
        OGUIWidget::draw(x, y, width, height);
        OGUIArea* area= (OGUIArea*)(this->area);
        NVGcontext* vg= (area->window->vg);
        // Draw box square
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x+1,y+1, width-2,height-2, 4);
        if(area->window->mouse_x > x &&
                area->window->mouse_x < x+width &&
                area->window->mouse_y > y &&
                area->window->mouse_y < y + height
                ) {
            nvgFillColor(vg, nvgRGBA(80, 80, 80, 255));
            area->window->setCursor(3);
            if (area->window->mouse_state == GLFW_PRESS && actual_press_status != GLFW_PRESS){
                actual_press_status = GLFW_PRESS;
            }else if(area->window->mouse_state == GLFW_RELEASE && actual_press_status == GLFW_PRESS){
                actual_press_status = GLFW_RELEASE;
                if(btn_click_callback!=NULL)
                    btn_click_callback();
            }
        }else {
            nvgFillColor(vg, nvgRGBA(60, 60, 60, 255));
            area->window->setCursor(0);
        }
        nvgFill(vg);

        nvgBeginPath(vg);
        nvgRoundedRect(vg, x+1,y+1, width-1,height-1, 4);
        nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
        nvgStroke(vg);

        // Draw text
        nvgFontSize(vg, 20.0f);
        nvgFontFace(vg, "sans-bold");
        float tw = nvgTextBounds(vg, 0,0, title, NULL, NULL);
        nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
        nvgFillColor(vg, nvgRGBA(0,0,0,160));
        nvgText(vg, x+width*0.5f-tw*0.5f,y+height*0.5f-1,title, NULL);
        nvgFillColor(vg, nvgRGBA(255,255,255,160));
        nvgText(vg, x+width*0.5f-tw*0.5f,y+height*0.5f,title, NULL);

    }

    void OGUIButton::setCallBack(std::function<void()> func) {
        btn_click_callback= func;
    }

    OGUIButton::OGUIButton(const char* title)
    {
        this->title= title;
    }

} /* End of namespace OpenCVGUI */
