#include "OGUIButton.h"
#include "OGUIWidget.h"
#include "OGUIArea.h"
#include "OGUIWindow.h"

namespace OpenCVGUI {

    void OGUIButton::draw(int x, int y, int width)
    {
        OGUIWidget::draw(x, y, width);
        OGUIArea* area= (OGUIArea*)(this->area);
        NVGcontext* vg= (area->window->vg);
        // Draw box square
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x,y+1, width,height, 2);
        
        if(area->window->mouse_x > x &&
                area->window->mouse_x < x+width &&
                area->window->mouse_y > y &&
                area->window->mouse_y < y + height
                ) {

            NVGpaint bg = nvgBoxGradient(vg, x, y, width,height, 2,2, nvgRGBA(0,0,0,32), nvgRGBA(0,0,0,128));
            nvgFillPaint(vg, bg);
            //nvgFillColor(vg, nvgRGBA(255,255,255, 30));

            area->window->setCursor(3);
            if (area->window->mouse_state == GLFW_PRESS && actual_press_status != GLFW_PRESS){
                actual_press_status = GLFW_PRESS;
            }else if(area->window->mouse_state == GLFW_RELEASE && actual_press_status == GLFW_PRESS){
                actual_press_status = GLFW_RELEASE;
                if(btn_click_callback!=NULL)
                    btn_click_callback();
            }
        }else {
            //nvgFillColor(vg, nvgRGBA(255,255,255, 20));
            NVGpaint bg = nvgBoxGradient(vg, x, y, width,height, 2,2, nvgRGBA(255,255,255,5), nvgRGBA(0,0,0,128));
            nvgFillPaint(vg, bg);
            area->window->setCursor(0);
        }
        nvgFill(vg);



        // Draw text
        nvgFontSize(vg, 20.0f);
        nvgFontFace(vg, "sans-bold");
        float tw = nvgTextBounds(vg, 0,0, title, NULL, NULL);
        nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
        nvgFillColor(vg, nvgRGBA(0,0,0,255));
        nvgText(vg, x+width*0.5f-tw*0.5f,y+height*0.5f-1,title, NULL);
        nvgFillColor(vg, nvgRGBA(255,255,255,255));
        nvgText(vg, x+width*0.5f-tw*0.5f,y+height*0.5f,title, NULL);

    }

    void OGUIButton::setCallBack(std::function<void()> func) {
        btn_click_callback= func;
    }

    OGUIButton::OGUIButton(const char* title)
    {
        this->height= 30;
        this->title= title;
        actual_press_status= GLFW_RELEASE;
    }

} /* End of namespace OpenCVGUI */
