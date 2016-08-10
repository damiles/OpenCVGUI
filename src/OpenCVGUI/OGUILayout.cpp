#include "OGUILayout.h"
#include "OGUIWindow.h"

namespace OpenCVGUI {


void OGUILayout::updateScrollStatus(double xoffset,double yoffset){
    for(int i=0; i<areas.size(); i++) {
        areas.at(i)->updateScrollStatus(xoffset, yoffset);
    }
}


void OGUILayout::draw(int x, int y, int width, int height)
{
    this->x=x;
    this->y=y;
    this->width= width;
    this->height= height;
    
    OGUIArea::draw(x,y,width,height);
    if(areas.size()==0)    
        return;
    
    //NVGcontext* vg= (window->vg);

    // Draw the areas 
    for(int i=0; i<areas.size(); i++) {
        int ax, ay, aw, ah;
        // Draw basic line for separate each area
        //nvgBeginPath(vg);

        if (orientation) { // 1 row layout
            float h = this->area_sizes.at(i) * height;
            float y0 = 0;
            if (i > 0)
                y0 = this->area_sizes.at(i - 1) * height;

            //nvgRect(vg, x, y+y0, width,1);
            ax = x;
            ay = y + y0 + 1;
            aw = width;
            ah = h - 1;

            if (window->mouse_y < ay + 2 && window->mouse_y > ay - 2)
                window->setCursor(VRESIZE_CURSOR);

        } else { // column layout
            float w = this->area_sizes.at(i) * width;
            float x0 = 0;
            if (i > 0)
                x0 = this->area_sizes.at(i - 1) * width;

            //nvgRect(vg,   x+x0 ,y,  1, height);
            ax = x + x0 + 1;
            ay = y;
            aw = w - 1;
            ah = height;

            if (window->mouse_x < ax + 2 && window->mouse_x > ax - 2)
                window->setCursor(HRESIZE_CURSOR);

        }

        /*
        if(i>0){
            nvgFillColor(vg, nvgRGBA(50,50,50,192));
            nvgFill(vg);
        }*/
        areas.at(i)->draw(ax,ay,aw,ah);
    }
}

OGUILayout::OGUILayout(OGUIWindow* window, int orientation): OGUIArea(window)
{
    this->areas.clear();
    this->orientation= orientation;
}

void OGUILayout::addArea(OGUIArea* area){
    this->title= "Layout";
    this->areas.push_back(area);
    this->area_sizes.push_back(0);
    // set all areas with same size
    float sizeperarea= 1.0/(float)areas.size();
    for(int i=0; i<this->area_sizes.size(); i++)
        this->area_sizes.at(i)= sizeperarea;
    area->layout= this;
}
void OGUILayout::setLayoutSizes(vector<float> sizes) {
    area_sizes= sizes;
}

} /* End of namespace OpenCVGUI */
