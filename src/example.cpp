
#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include "OGUIWindow.h"
#include "OGUILayout.h"
#include "OGUIFormArea.h"
#include "OGUIImageArea.h"
#include "OGUIWidget.h"
#include "OGUIButton.h"
#include "OGUISlider.h"

using namespace OpenCVGUI;
using namespace cv;

void hello_btn_click(){
    cout << "HELLO clicked" << endl;
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
	OGUIWindow window(1024, 768, "Wellcome to OpenCVGUI 1");

	Mat frame;
	VideoCapture camera(0);
	if(!camera.isOpened()){
        cout << "Error, no camera found" << endl;
		return -1;
    }

    OGUIFormArea formArea(&window);
    OGUILayout layout1(&window, 1);
    window.addArea(&formArea);
    window.addArea(&layout1);

    OGUIButton button("Hello");
    button.setCallBack(hello_btn_click);
    formArea.addWidget(&button);

    OGUISlider slider("Threshold", 0, 255, 125);
    formArea.addWidget(&slider);

    OGUIButton button1("Exit");
    button1.setCallBack(exit_btn_click);
    formArea.addWidget(&button1);

    std::vector<float> sizes;
    sizes.push_back(0.3);
    sizes.push_back(0.7);
    window.getMainLayout()->setLayoutSizes(sizes);

    OGUIImageArea imageArea1(&window);
    OGUIImageArea imageArea2(&window);
    layout1.addArea(&imageArea1);
    layout1.addArea(&imageArea2);
    
    camera >> frame;
    //frame= imread("../resources/icon1.png");
    imageArea1.setImage(frame);

    Mat img= imread("../resources/image1.jpg");
    imageArea2.setImage(img);

    while(true){
        //camera >> frame;
        //imageArea1.setImage(frame);
        window.update();
        //cvWaitKey(10);
        if(quit==1)
            break;
    }

//	destroyWindow("frame");


	return 1;
}

