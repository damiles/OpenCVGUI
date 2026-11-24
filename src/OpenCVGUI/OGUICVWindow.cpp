/**
 *  Created by damiles on 27/09/16.
 */

#include "OGUICVWindow.h"

namespace OpenCVGUI {

    OGUICVWindow::OGUICVWindow(int width,int height,const char* title, int layoutOrientation): OGUIWindow(width, height, title, layoutOrientation)
    {
        this->formArea= own(new OGUIFormArea(this));
        this->layout.push_back(own(new OGUILayout(this, 1)));
        this->layout.push_back(own(new OGUILayout(this, 1)));

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
    void OGUICVWindow::imshow(const string& area_title, const Mat* img) {
        imshow(area_title, *img);
    }

    void OGUICVWindow::imshow(const string& area_title, const Mat& img) {
        std::lock_guard<std::mutex> lock(_areas_mutex);
        /// Look for area
        OGUIArea* area=findArea(area_title);

        /// create or redraw
        if(area==NULL){
            // create new area and ad
            OGUIImageArea *ia= own(new OGUIImageArea(this, area_title));
            ia->setImage(img);
            showArea(ia);
        }else{
            OGUIImageArea *ia= (OGUIImageArea*)area;
            ia->setImage(img);
        }
    }

    void OGUICVWindow::addFormWidget(OGUIWidget *widget) {
        formArea->addWidget(widget);
    }

    void OGUICVWindow::plot3D(const string& area_title, const Mat* data) {
        plot3D(area_title, *data);
    }

    void OGUICVWindow::plot3D(const string& area_title, const Mat& data) {
        std::lock_guard<std::mutex> lock(_areas_mutex);
        /// Look for area
        OGUIArea* area=findArea(area_title);
        /// create or redraw
        if(area==NULL){
            // create new area and ad
            OGUI3D *ia= own(new OGUI3D(this, area_title, data));
            showArea(ia);
        }else{
            OGUI3D *ia= (OGUI3D*)area;
            // ToDo replot new 3D
            ia->replot(data);
        }
    }

    void OGUICVWindow::plot2D(const string& area_title, const Mat* data, const std::vector<string>& labels, float xstep, int plot_type) {
        plot2D(area_title, *data, labels, xstep, plot_type);
    }

    void OGUICVWindow::plot2D(const string& area_title, const Mat& data, const std::vector<string>& labels, float xstep, int plot_type) {
        std::lock_guard<std::mutex> lock(_areas_mutex);
        /// Look for area
        OGUIArea* area=findArea(area_title);
        /// create or redraw
        if(area==NULL){
            // create new area and ad
            OGUIPlotArea *ia= own(new OGUIPlotArea(this, area_title, data, labels, xstep, plot_type));
            showArea(ia);
        }else{
            OGUIPlotArea *ia= (OGUIPlotArea*)area;
            ia->replot(data, labels, xstep);
        }
    }

    OGUIArea *OGUICVWindow::findAreaByTitle(string title) {
        std::lock_guard<std::mutex> lock(_areas_mutex);
        return findArea(title);
    }

    OGUIArea *OGUICVWindow::findArea(string title) {
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

    /// Keep ownership of an area created by the window
    template<typename T> T* OGUICVWindow::own(T* area) {
        _owned_areas.push_back(std::unique_ptr<OGUIArea>(area));
        return area;
    }

    /// Add new area to the stacked layouts, layouts are only modified in the UI thread
    void OGUICVWindow::showArea(OGUIArea *area) {
        OGUILayout* l= layout.at(areas_showing.size()%2);
        areas_showing.push_back(area);
        runOnUIThread([l, area]() {
            l->addArea(area);
        });
    }

    bool OGUICVWindow::setImShowMouseClickCallBack(string area_title, std::function<void(int, int)> func) {
        std::lock_guard<std::mutex> lock(_areas_mutex);
        /// Look for area
        OGUIArea* area=findArea(area_title);
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
