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

void hello_btn_click(){
    cout << "2D Plot Demo" << endl;
    sayHello= true;
}

void cv_process() {

    Mat scatter_data(4,20,CV_32FC2);
    for(int s=0; s<4; s++){
        float x_mean= 0.5-(rand()%100)/100.0f;
        float y_mean= 0.5-(rand()%100)/100.0f;
        for(int p=0; p<20; p++){
            scatter_data.at<Vec2f>(s,p)[0]= x_mean + (50-(rand()%100))/1000.0f;
            scatter_data.at<Vec2f>(s,p)[1]= y_mean + (50-(rand()%100))/1000.0f;
        }
    }


    Mat lines_data(2,11,CV_32FC2);
    lines_data.at<Vec2f>(0,0)[0]= 10;
    lines_data.at<Vec2f>(0,0)[1]= 10;

    lines_data.at<Vec2f>(0,1)[0]= 12;
    lines_data.at<Vec2f>(0,1)[1]= 7;

    lines_data.at<Vec2f>(0,2)[0]= 15;
    lines_data.at<Vec2f>(0,2)[1]= 7;

    lines_data.at<Vec2f>(0,3)[0]= 13;
    lines_data.at<Vec2f>(0,3)[1]= 4;

    lines_data.at<Vec2f>(0,4)[0]= 15;
    lines_data.at<Vec2f>(0,4)[1]= 0;

    lines_data.at<Vec2f>(0,5)[0]= 10;
    lines_data.at<Vec2f>(0,5)[1]= 3;

    lines_data.at<Vec2f>(0,6)[0]= 5;
    lines_data.at<Vec2f>(0,6)[1]= 0;

    lines_data.at<Vec2f>(0,7)[0]= 7;
    lines_data.at<Vec2f>(0,7)[1]= 4;

    lines_data.at<Vec2f>(0,8)[0]= 5;
    lines_data.at<Vec2f>(0,8)[1]= 7;

    lines_data.at<Vec2f>(0,9)[0]= 8;
    lines_data.at<Vec2f>(0,9)[1]= 7;

    lines_data.at<Vec2f>(0,10)[0]= 10;
    lines_data.at<Vec2f>(0,10)[1]= 10;

    for(int i=0; i< 11; i++){
        double a= i*(2*M_PI)/10.0f;
        lines_data.at<Vec2f>(1,i)[0]= 10 + 10*cos(a);
        lines_data.at<Vec2f>(1,i)[1]= 5 + 10*sin(a);
    }

    while(app_is_running) {
        cap >> frame;
        window->imshow("camera", &frame);

        Mat g_data, data;
        cvtColor(frame, g_data, CV_BGR2GRAY);
        g_data.convertTo(data, CV_32F, 1/255.0);
        resize(data, data, Size(50,10));
        window->plot2D("2D plot Series", &data, {"g1","g2","g3","g4","g5","g6","g7","g8","g9", "g10"}, 1, OpenCVGUI::PLOT_SERIES);

        // 20 data 4 series (each channel is x and y)
        window->plot2D("2D plot Scatter", &scatter_data, {"g1","g2","g3","g4"}, 1, OpenCVGUI::PLOT_SCATTER);

        window->plot2D("2D plot Lines", &lines_data, {"star","circle"}, 1, OpenCVGUI::PLOT_LINE);

        resize(data, data, Size(20,3));
        window->plot2D("2D plot Bars", &data, {"g1","g2","g3"}, 1, OpenCVGUI::PLOT_BARS);

        if(sayHello) {
            window->popup("2D Plot Demo", "In this sample you can check different 2d plot types", POPUP_ALERT);
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

    OGUIButton button1("2D Plot Demo");
    button1.setCallBack(hello_btn_click);
    window->addFormWidget(&button1);

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

