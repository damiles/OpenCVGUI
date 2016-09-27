//
// Created by damiles on 27/09/16.
//

#include "OGUICVWindow.h"

namespace OpenCVGUI {

    OGUICVWindow::OGUICVWindow(int width,int height,const char* title, int layoutOrientation): OGUIWindow(width, height, title, layoutOrientation)
    {
        this->formArea= new OGUIFormArea(this);
        this->layout.push_back(new OGUILayout(this, 1));
        this->layout.push_back(new OGUILayout(this, 1));

        this->addArea(formArea);
        this->addArea(layout.at(0));
        this->addArea(layout.at(1));

        std::vector<float> sizes;
        sizes.push_back(0.2);
        sizes.push_back(0.4);
        sizes.push_back(0.4);
        this->getMainLayout()->setLayoutSizes(sizes);

    }

    /// Show image into layouts
    void OGUICVWindow::imshow(string area_title, void *img) {
        /// Look for area
        OGUIArea* area=NULL;
        for(int i=0; i<areas_showing.size(); i++){
            OGUIArea* tmp= areas_showing.at(i);
            if(tmp->title.compare(area_title)==0){
                area= tmp;
                break;
            }
        }

        if(area==NULL){
            // create new area and ad
            OGUIImageArea *ia= new OGUIImageArea(this, area_title);
            ia->setImage((Mat*)img);
            layout.at(areas_showing.size()%2)->addArea(ia);
            areas_showing.push_back((OGUIArea*)ia);
        }else{
            OGUIImageArea *ia= (OGUIImageArea*)area;
            ia->setImage((Mat*)img);
        }
    }

    void OGUICVWindow::addFormWidget(OGUIWidget *widget) {
        formArea->addWidget(widget);
    }


}
