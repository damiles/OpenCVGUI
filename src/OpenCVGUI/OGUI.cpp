//
// Created by damiles on 23/09/16.
//

#include "OGUI.h"
#include <thread>

namespace OpenCVGUI{

    vector<OGUIWindow*> windows;
	bool _app_is_running= true;

    OGUICVWindow* namedWindow(const char* title, int width,int height)
    {
        OGUICVWindow* w= new OGUICVWindow(width, height, title);
        windows.push_back(w);
        return w;
    }

	void add_window(OGUIWindow* window) {
		windows.push_back(window);
	}

    void app_run(std::function<void()> process_function)
    {
		_app_is_running = true;
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
		cout << "Closing app" << endl;
		_app_is_running= false;
        t1.join();
		cout << "Thread closed" << endl;
    }

	bool app_is_running() {
		return _app_is_running;
	}
}
