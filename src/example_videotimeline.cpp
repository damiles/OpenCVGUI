#include <iostream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <chrono>
#include <stdlib.h> 
#include <omp.h>

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
#include "OGUIVideoTimeline.h"



using namespace OpenCVGUI;
using namespace cv;

Mat src, gray, output;
Mat black;
vector<VideoCapture*> cap;
OGUIWindow *window;
vector<OGUIImageArea*> imageAreas;
OGUIVideoTimeline *timeline;
double fps;

Mat frame;

void hello_btn_click(){
    cout << "HELLO OGUI" << endl;
}

void cv_process() {
    int time_diff=0;
    int time=0;
    int delay = 1000 / fps;

    while(app_is_running) {
        
        window->updatePerfGraph();
        if (timeline->is_playing()){
            std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
            timeline->set_time_pos(time);
            #pragma omp for
            for(int i=0; i<cap.size(); i++){
                // cap[i]->set(CAP_PROP_POS_MSEC, 1000*timeline->get_time_pos());
                // cap[i]->set(CAP_PROP_POS_MSEC, time);
                cap[i]->read(frame);
                if(frame.empty())
                {
                    imageAreas[i]->setImage(&black);
                }else{
                    imageAreas[i]->setImage(&frame);
                }
            }
            
            time+=delay;
            std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
            time_diff = std::chrono::duration_cast<std::chrono::milliseconds>( t2 - t1 ).count();
            // wait for next frame
            this_thread::sleep_for(chrono::milliseconds(delay - time_diff));
        }else{
            time = timeline->get_time_pos();
            #pragma omp for
            for(int i=0; i<cap.size(); i++){
                cap[i]->set(CAP_PROP_POS_MSEC, time);
                cap[i]->read(frame);
                if(frame.empty())
                {
                    imageAreas[i]->setImage(&black);
                }else{
                    imageAreas[i]->setImage(&frame);
                }
            } 
        }
        
    }
}

int main( int argc, const char* argv[] )
{
    srand (time(NULL));
	OpenCVGUI::init();

    window= new OGUIWindow(1024, 768, "Wellcome to opencv GUI", 1);
    window->showPerfGraph(true);
	OGUILayout layout1(window, 0);
    OGUILayout layout2(window, 0);
    window->addArea(&layout1);
    window->addArea(&layout2);

    std::vector<float> sizes;
    sizes.push_back(0.8);
    sizes.push_back(0.2);
    window->getMainLayout()->setLayoutSizes(sizes);
    
    #pragma omp for
    for(int i=1; i<argc; i++){
        VideoCapture* c= new VideoCapture();
        c->open(argv[i]);
        cap.push_back(c);
        fps = c->get(CAP_PROP_FPS);

        OGUIImageArea* imageArea= new OGUIImageArea(window, "Input track");
        layout1.addArea(imageArea);
        imageAreas.push_back(imageArea);
    }
    

    timeline= new OGUIVideoTimeline(window, "Video Timeline");
    layout2.addArea(timeline);
    for(int i=0; i<cap.size(); i++){
        VideoCapture* c= cap[i];
        Track* track = new Track("Track", c);
        int start = rand()%20;
        track->add_marker("Test", start, start+(rand()%30), nvgRGB(rand()%255, rand()%255, rand()%255));
        timeline->addTrack(track);
        c->read(src);

        imageAreas[i]->setImage(&src);

    }
    // Track* track = new Track("Track test", cap);
    // timeline->addTrack(track);

    // Track* track2 = new Track("Track 2 test", cap);
    // timeline->addTrack(track2);
    
    *cap[0] >> src;
    black = Mat::zeros(src.rows, src.cols, CV_8UC3 );
    // imageArea1->setImage(&src);
    // cvtColor( src, gray, COLOR_RGB2GRAY );
    // imageArea2->setImage(&gray);

    OpenCVGUI::windows.push_back(window);
    OpenCVGUI::app_run(cv_process);

	return 1;
}

