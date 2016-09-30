#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>

#include "OGUI.h"
#include "OGUICVWindow.h"
#include "OGUILayout.h"
#include "OGUIFormArea.h"
#include "OGUIImageArea.h"
#include "OGUIWidget.h"
#include "OGUIButton.h"
#include "OGUISlider.h"
#include "OGUITextInput.h"
#include "OGUIRadioButtons.h"




using namespace OpenCVGUI;
using namespace cv;

Mat src, gray, output;
OGUICVWindow *window;
VideoCapture cap;
Mat frame;

void on_change_slider_threshold(double value){
    threshold( gray, output, (int)value, 255, CV_THRESH_BINARY);
    window->imshow("threshold", &output);
}

void hello_btn_click(){
    cout << "HELLO OGUI" << endl;
}

void cv_process() {
    while(app_is_running) {
        cap >> frame;
        window->imshow("camera", &frame);
    }
}

int main( int argc, const char* argv[] )
{
    srand (time(NULL));
	OpenCVGUI::init();

    window= OpenCVGUI::namedWindow("Wellcome to OpenCVGUI Example 2");

	src= imread("../resources/lena.png");

    OGUISlider slider("Threshold", 0, 255, 125);
    slider.setCallBack(on_change_slider_threshold);
    window->addFormWidget(&slider);

    OGUIButton button1("Hello");
    button1.setCallBack(hello_btn_click);
    window->addFormWidget(&button1);

    window->imshow("input", &src);
    
    cvtColor( src, gray, COLOR_RGB2GRAY );
    threshold( gray, output, 125, 255, CV_THRESH_BINARY);
    window->imshow("threshold", &output);

    Mat data;
    Mat indata= imread("../resources/3d.png", CV_LOAD_IMAGE_GRAYSCALE);
    window->imshow("3dindata", &indata);
    indata.convertTo(data, CV_32F, 1/255.0);
    //resize(data, data, Size(30,30));
    window->plot3D("3D plot", &data);

    cap.open(0);
    if(cap.isOpened()){
        cap >> frame;
        window->imshow("camera", &frame);
    }else{
        window->imshow("camera", &gray);
    }

    OpenCVGUI::app_run(cv_process);

	return 1;
}

