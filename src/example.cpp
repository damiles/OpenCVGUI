
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp> 
#include "OGUIWindow.h"
#include "OGUILayout.h"

using namespace OpenCVGUI;
using namespace cv;

int main( int argc, const char* argv[] )
{
    srand (time(NULL));
	OpenCVGUI::init();
	OGUIWindow window(640, 480, "Wellcome to OpenCVGUI 1");
	std::cout << "Wellcome to OpenCVGUI" << std::endl;

	Mat frame;
	VideoCapture camera(0);
	if(!camera.isOpened()){
        cout << "Error, no camera found" << endl;
		return -1;
    }

    OGUILayout layout(&window, 1);
    OGUILayout layout1(&window, 1);
    window.addArea(&layout);
    window.addArea(&layout1);
    
    OGUILayout layout2(&window, 1);
    OGUILayout layout3(&window, 1);
    layout1.addArea(&layout2);
    layout1.addArea(&layout3);
    
//	namedWindow("frame");
	/*char c;
	for(;;)
	{
		camera >> frame;
		//imshow("frame", frame);
		//c = (char)waitKey(10);
	     //   if (c == 27) break;
	}*/
    
    while(true){
        camera >> frame;
    }
    
//	destroyWindow("frame");
	glfwTerminate();

	return 1;
}

