#include "OGUIButton.h"
#include "OGUIWidget.h"
#include "OGUIArea.h"
#include "OGUIWindow.h"

#include <GL/glew.h>
#ifdef __APPLE__
#   define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
#include "nanovg.h"

namespace OpenCVGUI {

    void OGUIButton::draw(int x, int y, int width)
    {
        OGUIWidget::draw(x, y, width);
        OGUIArea* area= (OGUIArea*)(this->area);
        NVGcontext* vg= (NVGcontext*)(area->window->vg);
        // Draw box square
        nvgBeginPath(vg);
        nvgRoundedRect(vg, x+1,y+1, width-2,height-2, 3);
        
        if(area->window->mouse_x > x &&
                area->window->mouse_x < x+width &&
                area->window->mouse_y > y &&
                area->window->mouse_y < y + height
                ) {

//            NVGpaint bg = nvgLinearGradient(vg, x, y, x,y+height, nvgRGBA(255,255,255,24), nvgRGBA(0,0,0,24));
//            nvgFillPaint(vg, bg);
            nvgFillColor(vg, nvgRGBA(50,53,58,255));

            area->window->setCursor(HAND_CURSOR);
            if (area->window->mouse_state == GLFW_PRESS && actual_press_status != GLFW_PRESS){
                actual_press_status = GLFW_PRESS;
            }else if(area->window->mouse_state == GLFW_RELEASE && actual_press_status == GLFW_PRESS){
                actual_press_status = GLFW_RELEASE;
                if(btn_click_callback!=NULL)
                    btn_click_callback();
            }
        }else {
            nvgFillColor(vg, nvgRGBA(50,53,58,180));
//            NVGpaint bg = nvgLinearGradient(vg, x, y, x,y+height, nvgRGBA(255,255,255,32), nvgRGBA(0,0,0,32));
//            nvgFillPaint(vg, bg);
            area->window->setCursor(DEFAULT_CURSOR);
        }
        nvgFill(vg);

        nvgBeginPath(vg);
        nvgRoundedRect(vg, x+0.5f,y+0.5f, width-1,height-1, 3.5);
        nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
        nvgStroke(vg);

        // Draw text
        nvgFontSize(vg, 20.0f*area->window->font_scale);
        nvgFontFace(vg, "sans");
        float tw = nvgTextBounds(vg, 0,0, title, NULL, NULL);
        nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
        nvgFillColor(vg, nvgRGBA(0,0,0,255));
        nvgText(vg, x+width*0.5f-tw*0.5f,y+height*0.5f-1,title, NULL);
        nvgFillColor(vg, nvgRGBA(255,255,255,180));
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

    void OGUIButton::changeTitle(const char *title) {
        this->title= title;
    }

} /* End of namespace OpenCVGUI */
