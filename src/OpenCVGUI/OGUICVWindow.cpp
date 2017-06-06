/**
 *  Created by damiles on 27/09/16.
 */

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
        OGUIArea* area=findAreaByTitle(area_title);

        /// create or redraw
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

    void OGUICVWindow::plot3D(string area_title, void *data) {
        /// Look for area
        OGUIArea* area=findAreaByTitle(area_title);
        /// create or redraw
        if(area==NULL){
            // create new area and ad
            OGUI3D *ia= new OGUI3D(this, area_title, data);
            layout.at(areas_showing.size()%2)->addArea(ia);
            areas_showing.push_back((OGUIArea*)ia);
        }else{
            OGUI3D *ia= (OGUI3D*)area;
            // ToDo replot new 3D
            ia->replot(data);
        }
    }

    void OGUICVWindow::plot2D(string area_title, void *data, std::vector<string> labels, float xstep, int plot_type) {
        /// Look for area
        OGUIArea* area=findAreaByTitle(area_title);
        /// create or redraw
        if(area==NULL){
            // create new area and ad
            OGUIPlotArea *ia= new OGUIPlotArea(this, area_title, data, labels, xstep, plot_type);
            layout.at(areas_showing.size()%2)->addArea(ia);
            areas_showing.push_back((OGUIArea*)ia);
        }else{
            OGUIPlotArea *ia= (OGUIPlotArea*)area;
            ia->replot(data, labels, xstep);
        }
    }

    OGUIArea *OGUICVWindow::findAreaByTitle(string title) {
        OGUIArea* area= NULL;
        for(int i=0; i<areas_showing.size(); i++){
            OGUIArea* tmp= areas_showing.at(i);
            if(tmp->title.compare(title)==0){
                area= tmp;
                break;
            }
        }
        return area;
    }

    bool OGUICVWindow::setImShowMouseClickCallBack(string area_title, std::function<void(int, int)> func) {
        /// Look for area
        OGUIArea* area=findAreaByTitle(area_title);
        /// create or redraw
        if(area==NULL){
            return false;
        }else{
            OGUIImageArea *ia= (OGUIImageArea*)area;
            ia->setMouseClickCallBack(func);
            return true;
        }
    }


}
