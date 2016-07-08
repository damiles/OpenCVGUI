
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

using namespace OpenCVGUI;
using namespace cv;

void hello_btn_click(){
    cout << "HELLO clicked" << endl;
}

int quit=0;

void exit_btn_click(){
    quit=1;
}

int main( int argc, const char* argv[] )
{
    srand (time(NULL));
	OpenCVGUI::init();
	OGUIWindow window(1024, 768, "Wellcome to OpenCVGUI 1");
	std::cout << "Wellcome to OpenCVGUI" << std::endl;

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
    
//	namedWindow("frame");
	/*char c;
	for(;;)
	{
		camera >> frame;
		//imshow("frame", frame);
		//c = (char)waitKey(10);
	     //   if (c == 27) break;
	}*/

    camera >> frame;
    imageArea1.setImage(frame);

    while(true){
        camera >> frame;
        imageArea1.setImage(frame);
        cvWaitKey(10);
        if(quit==1)
            break;
    }

//	destroyWindow("frame");


	return 1;
}

