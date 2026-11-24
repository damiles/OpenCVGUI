//
// Created by damiles on 25/11/16.
//

#ifndef OPENCVGUI_OGUIFILEBROWSER_H
#define OPENCVGUI_OGUIFILEBROWSER_H


#include <vector>
#include <string>
#include <mutex>
#include <condition_variable>

#include "OGUIWindow.h"
#include "OGUIFile.h"




using namespace std;

namespace OpenCVGUI {

    class OGUIFileBrowser {
    public:
        OGUIFileBrowser(OGUIWindow* window);

        // Show the file browser and wait until it is closed, call it from the vision thread
        string open(string path, std::vector<string> filter);
        void draw();
        // Close the file browser without result, wakes up the thread waiting in open
        void cancel();

    private:
        OGUIWindow* _window;
        bool _is_file_browser_visible = false;
        OGUIFile* _file_browser_result;
        std::vector<OGUIFile*> _file_list;
        string _actual_folder;
        std::vector<string> _filter;
        std::vector <OGUIFile*> _system;

        char resolved_path[4096];

        // Synchronization with the thread waiting in open
        std::mutex _mutex;
        std::condition_variable _closed;
        bool _is_open= false;
        string _result_path;
        void close(OGUIFile* result);

        void readFolder(string path);
        void readSystem();
        int _preview_image=-1;
    };
}

#endif //OPENCVGUI_OGUIFILEBROWSER_H
