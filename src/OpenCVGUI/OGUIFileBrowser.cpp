//
// Created by damiles on 25/11/16.
//

#include "OGUIFileBrowser.h"

#include <GL/glew.h>
#ifdef __APPLE__
#   define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
#include "nanovg.h"


#include <tinydir.h>
#include "OGUIUtils.h"

namespace OpenCVGUI {

    OGUIFileBrowser::OGUIFileBrowser(OGUIWindow *window) {
        _window= window;
        _file_browser_result= NULL;
        _actual_folder="";
    }

    void OGUIFileBrowser::readFolder(string path) {
        _file_list.clear();
        tinydir_dir dir;
        if (tinydir_open_sorted(&dir, path.c_str()) == -1) {
            cout << "Error opening file" << endl;
            return;
        }
        for (int i = 0; i < dir.n_files; i++) {
            tinydir_file file;
            if (tinydir_readfile_n(&dir, &file, i) == -1) {
                cout << "Error getting file" << endl;
                return;
            }

            printf("%s", file.name);
            if (file.is_dir) {
                printf("/");
            }
            OGUIFile *oguifile= new OGUIFile(_window, file.name, file.path, file.is_dir, file.extension, 0);
            _file_list.push_back(oguifile);

            printf("\n");

        }

        tinydir_close(&dir);
    }

    string OGUIFileBrowser::open(string path, vector<string> filter) {
        _filter= filter;
        _actual_folder= path;
        if (_window->getStatus() != 0) {

            readFolder(path);

            // Now we have the list dir, then we can draw all data.
            _is_file_browser_visible = true;

            while (_is_file_browser_visible && _window->getStatus()) {
                usleep(1000);// Wait a milisec
            }
        }
        if (_file_browser_result != NULL){
            return _file_browser_result->_path+_file_browser_result->_file_name;
        }else{
            return "";
        }

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
                _file_browser_result= NULL;
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


            int x=210;
            int y=50;
            int max_height= _window->getWindowHeight()-80;
            for(int i=0; i<_file_list.size(); i++){
                OGUIFile *file= _file_list.at(i);
                if( file->draw(vg, x, y, _window->mouse_x, _window->mouse_y) ){ // is clicked
                    _file_browser_result= file;
                    if(_file_browser_result!=NULL) {
                        if (_file_browser_result->_is_dir
                            && _actual_folder.compare(_file_browser_result->_path) != 0) {
                            readFolder(_file_browser_result->_path);
                            _actual_folder= _file_browser_result->_path;
                            _file_browser_result = NULL;
                        }
                    }
                }
                y+=22;
                if(y>=max_height){
                    x+=202;
                    y=50;
                }
            }

            if(_file_browser_result!=NULL) {
                if (_file_browser_result->_is_dir == false) {
                    // show file in header
                    nvgFontSize(vg, 18.0f);
                    nvgFontFace(vg, "sans-bold");
                    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
                    nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
                    nvgText(vg, 209, _window->getWindowHeight()-32, _file_browser_result->_file_name.c_str(), NULL);
                    nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
                    nvgText(vg, 210, _window->getWindowHeight()-30, _file_browser_result->_file_name.c_str(), NULL);
                }
            }


        }
    }
}