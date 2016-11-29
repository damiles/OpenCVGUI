//
// Created by damiles on 25/11/16.
//

#ifndef OPENCVGUI_OGUIFILEBROWSER_H
#define OPENCVGUI_OGUIFILEBROWSER_H


#include <vector>
#include <string>

#include "OGUIWindow.h"
#include "OGUIFile.h"




using namespace std;

namespace OpenCVGUI {

    class OGUIFileBrowser {
    public:
        OGUIFileBrowser(OGUIWindow* window);

        string open(string path, vector<string> filter);
        void draw();

    private:
        OGUIWindow* _window;
        bool _is_file_browser_visible = false;
        OGUIFile* _file_browser_result;
        vector<OGUIFile*> _file_list;
        string _actual_folder;
        vector<string> _filter;
        vector <OGUIFile*> _system;

        char resolved_path[4096];

        void readFolder(string path);
        void readSystem();
        int _preview_image=-1;
    };
}

#endif //OPENCVGUI_OGUIFILEBROWSER_H
