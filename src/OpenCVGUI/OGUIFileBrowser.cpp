//
// Created by damiles on 25/11/16.
//

#include "OGUIButton.h"
#include "OGUIWidget.h"
#include "OGUIArea.h"
#include "OGUIWindow.h"

#include <GL/glew.h>
#ifdef __APPLE__
#   define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
#include "nanovg.h"

#include "OGUIFileBrowser.h"
#include <tinydir.h>
#include "OGUIUtils.h"

namespace OpenCVGUI {

    OGUIFileBrowser::OGUIFileBrowser(OGUIWindow *window) {
        _window= window;
    }

    string OGUIFileBrowser::open(string path, vector<string> filter) {
        if (_window->getStatus() != 0) {

            tinydir_dir dir;
            if (tinydir_open_sorted(&dir, path.c_str()) == -1) {
                cout << "Error opening file" << endl;
                return "";
            }
            for (int i = 0; i < dir.n_files; i++) {
                tinydir_file file;
                if (tinydir_readfile_n(&dir, &file, i) == -1) {
                    cout << "Error getting file" << endl;
                    return "";
                }

                printf("%s", file.name);
                if (file.is_dir) {
                    printf("/");
                    OGUIFile oguifile(file.name, file.path, file.is_dir, file.extension, 0 );
                    _file_list.push_back(oguifile);
                }else{

                }
                printf("\n");

            }

            tinydir_close(&dir);

            // Now we have the list dir, then we can draw all data.
            _is_file_browser_visible = true;

            while (_is_file_browser_visible && _window->getStatus()) {
                usleep(1000);// Wait a milisec
            }
        }
        return _file_browser_result;
    }

    void OGUIFileBrowser::draw() {
        if (_is_file_browser_visible) {
            NVGcontext *vg = (NVGcontext *) _window->vg;
            nvgBeginPath(vg);
            nvgRect(vg, 0, 0, _window->getWindowWidth(), _window->getWindowHeight());
            nvgFillColor(vg, nvgRGBA(28, 30, 34, 255));
            nvgFill(vg);


            // sep line
            nvgBeginPath(vg);
            nvgMoveTo(vg, 200, 40);
            nvgLineTo(vg, 200, _window->getWindowHeight());
            nvgFillColor(vg, nvgRGBA(0,0,0,255));
            nvgFill(vg);

            nvgBeginPath(vg);
            nvgMoveTo(vg, 0, 40);
            nvgLineTo(vg, _window->getWindowWidth(), 40);
            nvgFillColor(vg, nvgRGBA(0,0,0,255));
            nvgFill(vg);

            nvgBeginPath(vg);
            nvgMoveTo(vg, 200, _window->getWindowHeight()-40);
            nvgLineTo(vg, _window->getWindowWidth(), _window->getWindowHeight()-40);
            nvgFillColor(vg, nvgRGBA(0,0,0,255));
            nvgFill(vg);

            // Draw accept and cancel buttons
            if(drawBasicButton(vg, this->_window, "Accept", _window->getWindowWidth()-100, _window->getWindowHeight()-30, 90, 20, _window->mouse_x, _window->mouse_y)){
                _is_file_browser_visible=0;
            }

            if(drawBasicButton(vg, this->_window, "Close", _window->getWindowWidth()-200, _window->getWindowHeight()-30, 90, 20, _window->mouse_x, _window->mouse_y)){
                _file_browser_result="";
                _is_file_browser_visible=0;
            }

            // Draw de devices we found
            // Draw text
            nvgFontSize(vg, 18.0f);
            nvgFontFace(vg, "sans-bold");
            nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
            nvgFillColor(vg, nvgRGBA(0,0,0,255));
            nvgText(vg, 9, 48, "System", NULL);
            nvgFillColor(vg, nvgRGBA(255,255,255,255));
            nvgText(vg, 10, 50,  "System", NULL);




            int x=0;
            int y=0;
            for(int i=0; i<_file_list.size(); i++){
                OGUIFile file= _file_list.at(i);
                // Draw text
                nvgFontSize(vg, 15.0f);
                nvgFontFace(vg, "sans");
                nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
                nvgFillColor(vg, nvgRGBA(0,0,0,255));
                nvgText(vg, x+209, y+48, file._file_name.c_str() , NULL);
                nvgFillColor(vg, nvgRGBA(255,255,255,255));
                nvgText(vg, x+210, y+50,  file._file_name.c_str(), NULL);
                y=y+20;
            }

        }
    }
}