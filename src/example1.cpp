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

bool sayHello=false;
void on_change_slider_threshold(double value){
    threshold( gray, output, (int)value, 255, CV_THRESH_BINARY);
    window->imshow("threshold", &output);
}

void hello_btn_click(){
    cout << "HELLO OGUI" << endl;
    sayHello= true;
}

void cv_process() {

    while(app_is_running) {
        cap >> frame;
        window->imshow("camera", &frame);

        Mat g_data, data;
        cvtColor(frame, g_data, CV_BGR2GRAY);
        g_data.convertTo(data, CV_32F, 1/255.0);
        resize(data, data, Size(50,10));
        window->plot2D("2D plot Series", &data, {"g1","g2","g3","g4","g5","g6","g7","g8","g9", "g10"}, 1, OpenCVGUI::PLOT_SERIES);

        if(sayHello) {
            window->popup("Hello World", "Hello to OpenCV GUI Sample", POPUP_ALERT);
            sayHello= false;
        }
    }
}

void input_mouse_click(int mouse_x, int mouse_y)
{
    cout << mouse_x << ", " << mouse_y << endl;
    circle(src, Point(mouse_x, mouse_y), 3, Scalar(0,255,0));
    window->imshow("input", &src);
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
    window->setImShowMouseClickCallBack("input", input_mouse_click);
    cvtColor( src, gray, COLOR_RGB2GRAY );
    threshold( gray, output, 125, 255, CV_THRESH_BINARY);
    window->imshow("threshold", &output);

    Mat data;
    Mat indata= imread("../resources/3d.png", CV_LOAD_IMAGE_GRAYSCALE);
    //   window->imshow("3dindata", &indata);
//    indata.convertTo(data, CV_32F, 1/255.0);
    gray.convertTo(data, CV_32F, 1/255.0);
    resize(data, data, Size(100,100));
    window->plot3D("3D plot", &data);


    resize(data, data, Size(10,10));
    //data= data.reshape(0,1);
    window->plot2D("2D plot", &data, {"g1","g2","g3","g4","g5","g6","g7","g8","g9", "g10"});

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

