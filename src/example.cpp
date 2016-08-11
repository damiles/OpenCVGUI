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
#include "OGUITextInput.h"
#include "OGUIRadioButtons.h"

#ifdef X11
#include <X11/Xlib.h>
#endif


using namespace OpenCVGUI;
using namespace cv;

VideoCapture camera;

void hello_btn_click(){
    cout << "HELLO clicked" << endl;
}

int quit=0;

void exit_btn_click(){
    cout << "Bye" << endl;
    quit=1;
}

void on_change_camera(int option){
    camera.open(option);
}

int main( int argc, const char* argv[] )
{
    srand (time(NULL));

	OpenCVGUI::init();
	OGUIWindow window(1024, 768, "Wellcome to OpenCVGUI 1");

	Mat frame;


    OGUIFormArea formArea(&window);
    OGUILayout layout1(&window, 1);
    window.addArea(&formArea);
    window.addArea(&layout1);

//    OGUISlider slider("Threshold", 0, 255, 125);
//    formArea.addWidget(&slider);

    OGUITextInput textInputId("Id");
    formArea.addWidget(&textInputId);

    OGUITextInput textInputName("Name");
    formArea.addWidget(&textInputName);

    OGUITextInput textInputEmail("Email");
    formArea.addWidget(&textInputEmail);

    vector<string> cameras;
    int num_cameras=0;
    for(num_cameras=0; num_cameras<10; num_cameras++){
        if(!camera.open(num_cameras)) {
            break;
        }else{
            stringstream ss;
            ss << "Camera " << num_cameras;
            cameras.push_back(ss.str());
        }
    }

    if(num_cameras==0){
        cout << "Error, no cameras found" << endl;
		return -1;
    }

    // Initialize with first camera
    camera.open(0);

    OGUIRadioButtons camera_list("Cameras", cameras);
    camera_list.setCallBack(on_change_camera);
    formArea.addWidget(&camera_list);

    OGUIButton button("Start Record");
    button.setCallBack(hello_btn_click);
    formArea.addWidget(&button);

    OGUIButton buttonStop("Stop Record");
    buttonStop.setCallBack(hello_btn_click);
    formArea.addWidget(&buttonStop);

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
//    frame= imread("../resources/icon1.png");
    imageArea1.setImage(frame);

#ifdef X11
    // make screen shot
    Display *display = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(display);

    XWindowAttributes gwa;

    XGetWindowAttributes(display, root, &gwa);
    int width = gwa.width;
    int height = gwa.height;


    XImage *image = XGetImage(display,root, 0,0 , width,height,AllPlanes, ZPixmap);
    Mat img= Mat( image->height, image->width, CV_8UC4, image->data);
    resize(img, img, Size(0,0), 0.25, 0.25);
    cvtColor(img, img, CV_BGRA2BGR);

//    imwrite("out.jpg", img);
//    cvNamedWindow("test", WINDOW_NORMAL);
//    imshow("test",img);
//    cvWaitKey(0);

#else
    Mat img= imread("../resources/image1.jpg");
#endif
    imageArea2.setImage(img);


    int count=0;
    while(true){
        camera >> frame;
        imageArea1.setImage(frame);

//        if(count%15==0) {
//            XImage *image = XGetImage(display, root, 0, 0, width, height, AllPlanes, ZPixmap);
//            Mat img = Mat(image->height, image->width, CV_8UC4, image->data);
//            resize(img, img, Size(0,0), 0.25, 0.25);
//            cvtColor(img, img, CV_BGRA2BGR);
//            imageArea2.setImage(img);
//        }
//        count++;

        window.update();
        //cvWaitKey(10);
        if(quit==1)
            break;
    }

//	destroyWindow("frame");


	return 1;
}

