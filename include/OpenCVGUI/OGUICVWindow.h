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
#include <mutex>

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
     * @param {Mat} image opencv mat, it is copied
     */
    void imshow(const string& area_title, const Mat& img);
    [[deprecated("use imshow(title, const cv::Mat&)")]]
    void imshow(const string& area_title, const Mat* img);

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

    /**
     * Add or update a 3D plot of a CV_32F Mat, values are the height
     * @param {string} area_title plot area title
     * @param {Mat} data opencv mat, it is copied
     */
    void plot3D(const string& area_title, const Mat& data);
    [[deprecated("use plot3D(title, const cv::Mat&)")]]
    void plot3D(const string& area_title, const Mat* data);

    /**
     * Add or update a 2D plot, each Mat row is a serie
     * @param {string} area_title plot area title
     * @param {Mat} data opencv mat, it is copied
     * @param {vector<string>} labels one label per serie
     * @param {float} xstep x distance between values
     * @param {int} plot_type one of PlotTypes
     */
    void plot2D(const string& area_title, const Mat& data, const std::vector<string>& labels, float xstep=1, int plot_type=0);
    [[deprecated("use plot2D(title, const cv::Mat&, labels, ...)")]]
    void plot2D(const string& area_title, const Mat* data, const std::vector<string>& labels, float xstep=1, int plot_type=0);

    std::vector<OGUIArea*> areas_showing;
    std::vector<OGUILayout*> layout;
    OGUIFormArea *formArea;



private:
    // Protects areas_showing, imshow and plots can be called from any thread
    std::mutex _areas_mutex;
    OGUIArea* findArea(string title);
    void showArea(OGUIArea* area);

};

}

#endif //OPENCVGUI_OGUICVWINDOW_H
