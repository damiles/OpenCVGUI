//
// Created by damiles on 25/11/16.
//

#ifndef OPENCVGUI_OGUIFILEBROWSER_H
#define OPENCVGUI_OGUIFILEBROWSER_H

#include <vector>
#include <string>

#include "OGUIFile.h"
#include "OGUIWindow.h"

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
        string _file_browser_result;
        vector<OGUIFile> _file_list;


    };
}

#endif //OPENCVGUI_OGUIFILEBROWSER_H
