#include "OGUIArea.h"
#include "OGUIWindow.h"
#include <stdlib.h> 
#include <time.h> 

namespace OpenCVGUI {




void OGUIArea::draw(int x, int y, int width, int height)
{
    this->x=x;
    this->y=y;
    this->width=width;
    this->height=height;
    
    NVGcontext* vg= (window->vg);
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

OGUIArea::OGUIArea(OGUIWindow* window)
{
    this->layout= NULL;
    this->window= window;
    
    r= 28;
    g= 30;
    b= 34;
    /*r= rand()%255;
    g= rand()%255;
    b= rand()%255;*/

}

} /* End of namespace OpenCVGUI */
