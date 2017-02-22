#include "OGUIFormArea.h"
#include "OGUIWidget.h"

namespace OpenCVGUI {


    void OGUIFormArea::addWidget(OGUIWidget* widget){
        widget->area= this;
        widgets.push_back(widget);
    }

    void OGUIFormArea::draw(int x, int y, int width, int height)
    {
        this->x=x;
        this->y=y;
        this->width= width;
        this->height= height;

        OGUIArea::draw(x,y,width,height);

        int xw= x+10;
        int yw= y+10;

        for(int i=0; i<widgets.size(); i++){
            OGUIWidget* widget= widgets.at(i);
            widget->draw(xw,yw, width-20);
            yw+= widget->height+5;
        }
        drawTitle();
    }

    OGUIFormArea::OGUIFormArea(OGUIWindow* window): OGUIArea(window)
    {
        _show_title_bar= false;
    }

} /* End of namespace OpenCVGUI */
