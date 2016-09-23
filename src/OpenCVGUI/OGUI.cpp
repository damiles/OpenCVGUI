//
// Created by damiles on 23/09/16.
//

#include "OGUI.h"
#include <thread>

namespace OpenCVGUI{

    vector<OGUIWindow*> windows;

    OGUIWindow* namedWindow(const char* title, int width,int height)
    {
        OGUIWindow* w= new OGUIWindow(width, height, title);
        windows.push_back(w);
        return w;
    }

    void imshow(const string title, cv::Mat img)
    {

    }

    void app_run(std::function<void()> process_function)
    {
        std::thread t1(process_function);

        while(true){
            bool close=false;
            for(int i=0; i<windows.size(); i++) {
                OGUIWindow* w= (OGUIWindow*)windows.at(i);
                close = close | !w->update();
            }
            if(close)
                break;
        }
        app_is_running= false;
        t1.join();
    }
}