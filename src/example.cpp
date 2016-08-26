#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
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

void on_change_slider_threshold(double value){
    threshold( gray, output, (int)value, 255, CV_THRESH_BINARY);
}

int quit=0;

void exit_btn_click(){
    cout << "Bye" << endl;
    quit=1;
}

int main( int argc, const char* argv[] )
{
    srand (time(NULL));
	OpenCVGUI::init();
	OGUIWindow window(1024, 768, "Wellcome to OpenCVGUI");

	OGUIFormArea formArea(&window);
    OGUILayout layout1(&window, 1);
    OGUILayout layout2(&window, 1);
    window.addArea(&formArea);
    window.addArea(&layout1);
    window.addArea(&layout2);

    std::vector<float> sizes;
    sizes.push_back(0.2);
    sizes.push_back(0.4);
    sizes.push_back(0.4);
    window.getMainLayout()->setLayoutSizes(sizes);

    src= imread("../resources/lena.png");

    OGUISlider slider("Threshold", 0, 255, 125);
    slider.setCallBack(on_change_slider_threshold);
    formArea.addWidget(&slider);

    OGUIButton button1("Exit");
    button1.setCallBack(exit_btn_click);
    formArea.addWidget(&button1);

    OGUIImageArea imageArea1(&window);
    OGUIImageArea imageArea2(&window);
    OGUIImageArea imageArea3(&window);
    layout1.addArea(&imageArea1);
    layout1.addArea(&imageArea2);
    layout2.addArea(&imageArea3);
    
    imageArea1.setImage(src);
    cvtColor( src, gray, COLOR_RGB2GRAY );
    threshold( gray, output, 125, 255, CV_THRESH_BINARY);
    imageArea2.setImage(output);
    imageArea3.setImage(gray);
    while(true){
        imageArea2.setImage(output);
        window.update();
        //cvWaitKey(10);
        if(quit==1)
            break;
    }

	return 1;
}

