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

#ifdef X11
#include <X11/Xlib.h>
#endif

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/stat.h>
#endif


using namespace OpenCVGUI;
using namespace cv;

VideoCapture camera0, camera1;
bool start_capturing=false;
int face_camera, device_camera;
int count_frame;
string id_session;
stringstream output_dir;
OGUITextInput *textInputId;

void stop_btn_click() {
    start_capturing= false;
    count_frame=0;
}

void start_btn_click(){
    cout << "Start clicked" << endl;
    // Create new ID session
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    stringstream id;
    std::chrono::milliseconds ms=std::chrono::duration_cast< std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch());
//    id << (now->tm_year + 1900)
//        << (now->tm_mon + 1)
//        << now->tm_mday
//        << now->tm_hour
//        << now->tm_min
//        << now->tm_sec
//        << ms.count();
    id << ms.count();
    id_session= id.str();
    textInputId->value= id_session;
    output_dir.str("");
    output_dir << "./" << id_session;
    cout << output_dir.str() << endl;
    // Create folders to save data
#ifdef _WIN32
    //create directory with windows commands
            CreateDirectory(output_dir_str().c_str(), NULL);
#else
    // create directory for unix based os
    mkdir(output_dir.str().c_str(), 0777);
#endif

    // Start capturing flag
    start_capturing= true;
}

int quit=0;

void exit_btn_click(){
    cout << "Bye" << endl;
    quit=1;
}

void on_change_camera(int option){
//    face_camera.open(option);
    face_camera= option;
}

void on_change_device_camera(int option){
//    device_camera.open(option);
    device_camera=option;
}

int main( int argc, const char* argv[] )
{
    srand (time(NULL));
    id_session= "123456";
    count_frame=0;
    face_camera=0;
    device_camera=1;
	OpenCVGUI::init();
	OGUIWindow window(1024, 768, "Wellcome to OpenCVGUI 1");

	Mat face_frame, device_frame;


    OGUIFormArea formArea(&window);
    OGUILayout layout1(&window, 1);
    window.addArea(&formArea);
    window.addArea(&layout1);

//    OGUISlider slider("Threshold", 0, 255, 125);
//    formArea.addWidget(&slider);


    vector<string> cameras;
    int num_cameras=0;
    for(num_cameras=0; num_cameras<10; num_cameras++){
        if(!camera0.open(num_cameras)) {
            break;
        }else{
            stringstream ss;
            ss << "Camera " << num_cameras;
            cameras.push_back(ss.str());
        }
    }

    if(num_cameras<2){
        cout << "Error, no enough cameras found" << endl;
		return -1;
    }

    // Initialize with first camera
    camera0.open(0);
    camera1.open(1);

    OGUIRadioButtons camera_list("Select Face Camera", cameras);
    camera_list.setCallBack(on_change_camera);
    formArea.addWidget(&camera_list);

    OGUIRadioButtons camera_device_list("Select Device Camera", cameras, 1);
    camera_device_list.setCallBack(on_change_device_camera);
    formArea.addWidget(&camera_device_list);



    textInputId=  new OGUITextInput("Id");
    formArea.addWidget(textInputId);

    OGUIButton button("Start Record");
    button.setCallBack(start_btn_click);
    formArea.addWidget(&button);

    OGUIButton buttonStop("Stop Record");
    buttonStop.setCallBack(stop_btn_click);
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
    
    camera0 >> face_frame;
//    frame= imread("../resources/icon1.png");
    imageArea1.setImage(face_frame);

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
    camera1 >> device_frame;
    imageArea2.setImage(device_frame);


    int count=0;
    // Start clock
    std::chrono::high_resolution_clock::time_point t= std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> mseconds;
    while(true){
        std::chrono::high_resolution_clock::time_point tnow= std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> mseconds = std::chrono::duration_cast<std::chrono::duration<double>>(tnow - t);
//        cout << " Time " << mseconds.count() << "sec " <<  endl;

        if(face_camera==0)
            camera0 >> face_frame;
        else
            camera1 >> face_frame;

        if(device_camera==0)
            camera0 >> device_frame;
        else
            camera1 >> device_frame;

        // If is 200ms save frames
        if(mseconds.count()>=0.2 && start_capturing){
            cout << "CAPTURED FRAME " << count_frame <<  " at " << mseconds.count() << "sec " <<  endl;
            stringstream output_face_file;
            output_face_file << "./" << id_session << "/frame_" << count_frame << ".jpg";


            stringstream output_device_file;
            output_device_file << "./" << id_session << "/device_" << count_frame << ".jpg";

            imwrite(output_face_file.str().c_str(), face_frame);
            imwrite(output_device_file.str().c_str(), device_frame);

            t= tnow;
            count_frame++;

        }
        imageArea1.setImage(face_frame);
        imageArea2.setImage(device_frame);

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

