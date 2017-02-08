#include "OGUILayout.h"
#include "OGUIWindow.h"

#include <GL/glew.h>
#ifdef __APPLE__
#   define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
#include "nanovg.h"

namespace OpenCVGUI {


void OGUILayout::updateScrollStatus(double xoffset,double yoffset){
    for(int i=0; i<areas.size(); i++) {
        areas.at(i)->updateScrollStatus(xoffset, yoffset);
    }
}

void OGUILayout::draw3d(int x, int y, int width, int height)
{
    float x0=0;
    float y0=0;
    for(int i=0; i<areas.size(); i++) {
        int ax, ay, aw, ah;
        if (orientation) { // 1 row layout
            ax = x;
            ay = y + y0 + 1;
            float h = this->area_sizes.at(i) * height;

            aw = width;
            ah = h - 1;

            y0+=h;
        }else{
            ax = x + x0 + 1;
            ay = y;
            float w = this->area_sizes.at(i) * width;

            aw = w - 1;
            ah = height;

            x0+=w;
        }
        areas.at(i)->draw3d(ax,ay,aw,ah);
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
    float x0=0;
    float y0=0;
    for(int i=0; i<areas.size(); i++) {
        int ax, ay, aw, ah;
        // Draw basic line for separate each area
        //nvgBeginPath(vg);

        if (orientation) { // 1 row layout
            //nvgRect(vg, x, y+y0, width,1);
            ax = x;
            ay = y + y0 + 1;


            if (window->mouse_y < ay + 2 && window->mouse_y > ay - 2
                && window->mouse_x < ax+aw && window->mouse_x > ax) {
                window->setCursor(VRESIZE_CURSOR);
                if (window->mouse_state == GLFW_PRESS){
                    id_area_pressed= i-1;
                    is_pressed= true;
                }
            }
            if(is_pressed) {
                float new_size= ((float) window->mouse_y / (float) height)- (areas.at(id_area_pressed)->y/(float)height);
                float diff= this->area_sizes.at(id_area_pressed) - new_size;

                // Calculate the new area size...
                this->area_sizes.at(id_area_pressed) = new_size;

                float num_other_areas= areas.size() - id_area_pressed - 1;
                for(int j=id_area_pressed+1; j<areas.size(); j++){
                    this->area_sizes.at(j)= this->area_sizes.at(j)+(diff/num_other_areas);
                }
            }

            float h = this->area_sizes.at(i) * height;

            aw = width;
            ah = h - 1;

            y0+=h;

        } else { // column layout

            ax = x + x0 + 1;
            ay = y;

            if (window->mouse_x < ax + 2 && window->mouse_x > ax - 2 &&
                window->mouse_y < ay+ah && window->mouse_y > ay) {
                window->setCursor(HRESIZE_CURSOR);
                if (window->mouse_state == GLFW_PRESS){
                    is_pressed= true;
                    id_area_pressed=i-1;
                }
            }
            if(is_pressed) {

                float new_size= ((float) window->mouse_x / (float) width)- (areas.at(id_area_pressed)->x/(float)width);
                float diff= this->area_sizes.at(id_area_pressed) - new_size;

                // Calculate the new area size...
                this->area_sizes.at(id_area_pressed) = new_size;

                float num_other_areas= areas.size() - id_area_pressed - 1;
                for(int j=id_area_pressed+1; j<areas.size(); j++){
                    this->area_sizes.at(j)= this->area_sizes.at(j)+(diff/num_other_areas);
                }
            }

            float w = this->area_sizes.at(i) * width;

            aw = w - 1;
            ah = height;

            x0+=w;
        }
        if (window->mouse_state == GLFW_RELEASE){
            is_pressed= false;
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
