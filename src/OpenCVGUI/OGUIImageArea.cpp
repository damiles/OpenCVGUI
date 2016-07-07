#include "OGUIImageArea.h"
#include "OGUIWindow.h"

namespace OpenCVGUI {


OGUIImageArea::OGUIImageArea(OGUIWindow* window): OGUIArea(window)
{
}


void OGUIImageArea::draw(int x, int y, int width, int height)
{
    this->x=x;
    this->y=y;
    this->width= width;
    this->height= height;

    OGUIArea::draw(x,y,width,height);
    NVGcontext* vg= (window->vg);

    for(int i=0; i<width-2; i+=40){
        for(int j=0; j<height-2; j+=20){
            nvgBeginPath(vg);
            int offset=20;
            if((j%40)==0)
                offset=0;
            int xs= x+i+offset+1;
            int ys= y+j+1;
            int w=20;
            int h=20;
            if(xs+20 > x+width-2)
                w= x+width-2-xs;
            if(ys+20 > y+height-2)
                h= y+height-2-ys;
            nvgRect(vg, xs, ys, w, h);
            nvgFillColor(vg, nvgRGBA(50,50,50,100));
            nvgFill(vg);
        }
    }

}


} /* End of namespace OpenCVGUI */
