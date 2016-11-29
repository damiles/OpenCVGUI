//
// Created by damiles on 20/10/16.
//

#include "OGUI.h"
#include "OGUISlider.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace OpenCVGUI;
using namespace cv;

VideoCapture cap;
Mat frame, gray;
double thr=125;
OGUICVWindow *window;
int first_info=0;
void cv_process() {
    while(app_is_running) {
        cap >> frame;
        window->imshow("camera", &frame);
        Mat output;
        cvtColor( frame, gray, COLOR_RGB2GRAY );
        threshold( gray, output, (int)thr, 255, CV_THRESH_BINARY);
        window->imshow("threshold", &output);
        if(first_info==0) {
            string file_string= window->openFileBrowser(".", {"*"});
            stringstream ss;
            ss << "Selected file: " << file_string << "\n\n";
            ss << "Please \"Accept\" to close this window";
            first_info = window->popup("Sample Info",
                                       ss.str().c_str(),
                                       POPUP_CONFIRM);
        }
    }
}

void on_change_slider_threshold(double value){
    thr= value;
}

int main( int argc, const char* argv[] )
{
    // Init GUI
    OpenCVGUI::init();

    // Create our window
    window= OpenCVGUI::namedWindow("Wellcome to OpenCVGUI Example 2");

    // Create the Forms we need and add to window
    OGUISlider slider("Threshold", 0, 255, 125);
    slider.setCallBack(on_change_slider_threshold);
    window->addFormWidget(&slider);

    // Start video capture
    cap.open(0);
    if(cap.isOpened()){
        cap >> frame;
        // show image
        window->imshow("camera", &frame);
    }else{
        return 0;
    }

    // Convert frame, threshodl and show
    cvtColor( frame, gray, COLOR_RGB2GRAY );
    Mat output;
    threshold( gray, output, 125, 255, CV_THRESH_BINARY);
    window->imshow("threshold", &output);

    // Start the Comptuer vision process
    OpenCVGUI::app_run(cv_process);

}