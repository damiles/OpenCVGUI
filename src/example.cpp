#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>

#include "OGUI.h"
#include "OGUIWindow.h"
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
OGUIWindow *window;
OGUIImageArea *imageArea1;
OGUIImageArea *imageArea2;
OGUIImageArea *imageArea3;
VideoCapture cap;
Mat frame;

void on_change_slider_threshold(double value){
    threshold( gray, output, (int)value, 255, CV_THRESH_BINARY);
    imageArea2->setImage(&output);
}

void hello_btn_click(){
    cout << "HELLO OGUI" << endl;
}

void cv_process() {
    while(app_is_running) {
        cap >> frame;
        imageArea3->setImage(&frame);
    }
}

int main( int argc, const char* argv[] )
{
    srand (time(NULL));
	OpenCVGUI::init();

    window= new OGUIWindow(1024, 768, "Wellcome to opencv GUI");
    //OGUIWindow* window1= OpenCVGUI::namedWindow("Wellcome to OpenCVGUI 2");

	OGUIFormArea formArea(window);
    OGUILayout layout1(window, 1);
    OGUILayout layout2(window, 1);
    window->addArea(&formArea);
    window->addArea(&layout1);
    window->addArea(&layout2);

    std::vector<float> sizes;
    sizes.push_back(0.2);
    sizes.push_back(0.4);
    sizes.push_back(0.4);
    window->getMainLayout()->setLayoutSizes(sizes);

    src= imread("../resources/lena.png");

    OGUISlider slider("Threshold", 0, 255, 125);
    slider.setCallBack(on_change_slider_threshold);
    formArea.addWidget(&slider);

    OGUIButton button1("Hello");
    button1.setCallBack(hello_btn_click);
    formArea.addWidget(&button1);

    imageArea1= new OGUIImageArea(window, "Input image");
    imageArea2= new OGUIImageArea(window,"Threshold");
    imageArea3= new OGUIImageArea(window, "Camera");
    layout1.addArea(imageArea1);
    layout1.addArea(imageArea2);
    layout2.addArea(imageArea3);
    
    imageArea1->setImage(&src);
    cvtColor( src, gray, COLOR_RGB2GRAY );
    threshold( gray, output, 125, 255, CV_THRESH_BINARY);
    imageArea2->setImage(&output);

    //imageArea3.setImage(gray);
    cap.open(0);
    if(cap.isOpened()){
        cap >> frame;
        imageArea3->setImage(&frame);
    }else{
        imageArea3->setImage(&gray);
    }

    OpenCVGUI::windows.push_back(window);
    OpenCVGUI::app_run(cv_process);

	return 1;
}

