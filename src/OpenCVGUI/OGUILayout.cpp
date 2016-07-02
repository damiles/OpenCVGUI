#include "OGUILayout.h"
#include "OGUIWindow.h"

namespace OpenCVGUI {




void OGUILayout::draw(int x, int y, int width, int height)
{
    this->x=x;
    this->y=y;
    this->width= width;
    this->height= height;
    
    OGUIArea::draw(x,y,width,height);
    if(areas.size()==0)    
        return;
    
    NVGcontext* vg= (window->vg);
    int numAreas= areas.size();
    int sizeperarea= width/numAreas;
    if(orientation) // 1: ROWS
        sizeperarea= height/numAreas;
    
    // Draw the areas 
    for(int i=0; i<areas.size(); i++){
        int ax,ay,aw,ah;
        // Draw basic line for separate each area
        if(i>0){
            nvgBeginPath(vg);
            if(orientation){
                nvgRect(vg, x, y+i*sizeperarea, width,1);
                ax=x;
                ay=y+i*sizeperarea+1;
                aw= width;
                ah= sizeperarea-1;
                
                if(window->mouse_y<ay+2 && window->mouse_y>ay-2)
                    window->setCursor(2);
                
            }else{
                nvgRect(vg,   x+i*sizeperarea ,y,  1, height);
                ax=x+i*sizeperarea+1;
                ay=y;
                aw= sizeperarea-1;
                ah= height;
                
                if(window->mouse_x<ax+2 && window->mouse_x>ax-2)
                    window->setCursor(1);
                
            }
            nvgFillColor(vg, nvgRGBA(50,50,50,192));
            nvgFill(vg);
        }
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
    area->layout= this;
}

} /* End of namespace OpenCVGUI */
