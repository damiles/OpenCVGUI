/**
 *  Created by damiles on 27/09/16.
 */

#ifndef OPENCVGUI_OGUICVWINDOW_H
#define OPENCVGUI_OGUICVWINDOW_H

#include "OGUIWindow.h"
#include "OGUIFormArea.h"
#include "OGUILayout.h"
#include "OGUIImageArea.h"
#include "OGUIWidget.h"
#include "OGUI3D.h"
#include "OGUIPlotArea.h"

namespace OpenCVGUI {
/**
 * Comptuer Vision template window
 */
class OGUICVWindow : public OGUIWindow{
public:
    /**
     * \brief Create new Computer Vision Template Window with lateral form and stacked areas
     *
     * This class allow create easy computer vision purposes windows where each new area
     * created is stacked. There are similar functions than OpenCV to show images (imshow) and
     * more missing areas for plotting and forms.
     *
     * @param {const char*} title of window to create
     * @param {int} width of window, 1024 by default
     * @param {int} height of window, 768 by default
     * @return {OGUICVWindow*} OGUICVWindow created
     */
    OGUICVWindow(int width,int height,const char* title, int layoutOrientation=0);

    /**
     * Add new image to window with title
     * @param {string} area_title image are title
     * @param {Mat} image opencv mat
     */
    void imshow(string area_title, void* img);

    /**
     * Add callback for mouse interaction to specific image area
     * @param {string} area title identifier
     * @param {function(int, int)} callback function
     * @return {bool} true if setup correctly
     */
    bool setImShowMouseClickCallBack(string area_title, std::function<void(int, int)> func);

    /**
     * Find an area by a specific title
     * @param {string} title of area to search
     * @return {OGUIArea*} pointer to area that is found. Null in other case
     */
    OGUIArea* findAreaByTitle(string title);

    void addFormWidget(OGUIWidget* widget);
    void plot3D(string area_title, void* data);
    void plot2D(string area_title, void* data, std::vector<string> labels, float xstep=1, int plot_type=0);

    std::vector<OGUIArea*> areas_showing;
    std::vector<OGUILayout*> layout;
    OGUIFormArea *formArea;



private:

};

}

#endif //OPENCVGUI_OGUICVWINDOW_H
