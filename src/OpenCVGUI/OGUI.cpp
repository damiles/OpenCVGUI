//
// Created by damiles on 23/09/16.
//

#include "OGUI.h"
#include <thread>

namespace OpenCVGUI{

    vector<OGUIWindow*> windows;
    bool app_is_running= true;

    OGUICVWindow* namedWindow(const char* title, int width,int height)
    {
        OGUICVWindow* w= new OGUICVWindow(width, height, title);
        windows.push_back(w);
        return w;
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
