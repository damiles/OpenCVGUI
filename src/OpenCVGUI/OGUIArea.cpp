#include "OGUIArea.h"
#include "OGUIWindow.h"
#include <stdlib.h> 
#include <time.h> 
#include "nanovg.h"
#include "OGUIUtils.h"

namespace OpenCVGUI {


void OGUIArea::draw3d(int x, int y, int width, int height){

}

void OGUIArea::draw(int x, int y, int width, int height)
{
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
    
    NVGcontext* vg= (NVGcontext*)(window->vg);
    // Window
	nvgBeginPath(vg);
	nvgRect(vg, x,y, width,height);
	//nvgFillColor(vg, nvgRGBA(28,30,34,255));
    nvgFillColor(vg, nvgRGBA(r,g,b,255));
	nvgFill(vg);


    nvgBeginPath(vg);
    nvgRect(vg, x, y, width,1);
    nvgRect(vg, x, y, 1,height);
    nvgFillColor(vg, nvgRGBA(75,75,75,192));
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgRect(vg, x, y+height-1, width,1);
    nvgRect(vg, x+width-1, y, 1,height);
    nvgFillColor(vg, nvgRGBA(0,0,0,192));
    nvgFill(vg);

}

void OGUIArea::drawTitle()
{
    NVGcontext* vg= (NVGcontext*)(window->vg);
    if(isMouseIn() && _show_title_bar) {
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

        if(isMaximized){
            if(drawBasicButton(vg, this->window, "\uF066", x+width-30, y, 30, 22, this->window->mouse_x, this->window->mouse_y, "icons", 16) && _count_delay>100){
                isMaximized= false;
                this->window->maximizeArea(NULL);
                _count_delay=0;
            }
        }else{
            if(drawBasicButton(vg, this->window, "\uF065", x+width-30, y, 30, 22, this->window->mouse_x, this->window->mouse_y, "icons", 16) && _count_delay>100){
                this->window->maximizeArea(this);
                isMaximized= true;
                _count_delay=0;
            }
        }
        _count_delay++;
    }
}

bool OGUIArea::isMouseIn()
{
    if(window->mouse_x>x && window->mouse_x<x+width && window->mouse_y>y && window->mouse_y<y+height)
        return true;
    else
        return false;
}

void OGUIArea::updateScrollStatus(double xoffset,double yoffset)
{
    return;
}

OGUIArea::OGUIArea(OGUIWindow* window)
{
    this->layout= NULL;
    this->window= window;
    isMaximized= false;

    r= 28;
    g= 30;
    b= 34;
    /*
    r= 76;
    g= 82;
    b= 89;

    r= 56;
    g= 62;
    b= 69;
    */
    /*r= rand()%255;
    g= rand()%255;
    b= rand()%255;*/

}

} /* End of namespace OpenCVGUI */
