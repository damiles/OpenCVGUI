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

bool show_popup_help=false;

void hello_btn_click(){
    cout << "3D Sample" << endl;
    show_popup_help= true;
}

void cv_process() {

    while(app_is_running) {
        if(show_popup_help) {
            window->popup("3D Sample", "In this sample shows how to plot 3D from OpenCV Mat and a custom 3D", POPUP_ALERT);
            show_popup_help= false;
        }
    }
}

void custom_3d(int x, int y, int width, int height)
{
    cout << "Drawing 3d" << endl;
}

int main( int argc, const char* argv[] )
{
	OpenCVGUI::init();

    window= OpenCVGUI::namedWindow("Wellcome to OpenCVGUI Example 3D");
    window->font_scale=1.2f;

	OGUIButton button1("3D Sample");
    button1.setCallBack(hello_btn_click);
    window->addFormWidget(&button1);

    Mat data;
    Mat indata= imread("../resources/3d.png", CV_LOAD_IMAGE_GRAYSCALE);
    blur(indata, indata, Size(3,3));
    window->imshow("3dindata", &indata);
    indata.convertTo(data, CV_32F, 1/255.0);
    //resize(data, data, Size(100,100));
    window->plot3D("3D plot", &data);

    Mat empty_data;
    window->plot3D("Custom 3D plot", &empty_data);
    OGUI3D* area3d= (OGUI3D*)window->findAreaByTitle("Custom 3D plot");
    area3d->externDraw3d= custom_3d;

    OpenCVGUI::app_run(cv_process);

	return 1;
}

