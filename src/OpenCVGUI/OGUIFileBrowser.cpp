//
// Created by damiles on 25/11/16.
//

// for realpath, ToDo: check if will work in windows
#include <stdlib.h>
#include <mntent.h>

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

    // ToDo (damiles) enable options to create folders and show/hide hidden files
    // ToDo (damiles) By default hide the hidden files
    OGUIFileBrowser::OGUIFileBrowser(OGUIWindow *window) {
        _window= window;
        _file_browser_result= NULL;
        _actual_folder="";
        readSystem();
    }

    void OGUIFileBrowser::readSystem() {
        struct mntent *ent;
        FILE *aFile;
        // ToDo (damiles): Windows and osx support
        aFile = setmntent("/proc/mounts", "r");
        if (aFile == NULL) {
            perror("setmntent");
            exit(1);
        }
        while (NULL != (ent = getmntent(aFile))) {
            if(ent->mnt_fsname[0]=='/'){
                string mnt_dir(ent->mnt_dir);
                string filename;
                std::size_t found = mnt_dir.find_last_of("/\\");
                filename= mnt_dir.substr(found+1) ;
                if(filename.compare("")==0)
                    filename="/";
                OGUIFile* file= new OGUIFile(_window, filename.c_str(),ent->mnt_dir, 2, "",0);
                _system.push_back(file);
            }
        }
        endmntent(aFile);
    }

    void OGUIFileBrowser::readFolder(string path) {
        realpath(path.c_str(), resolved_path);
        _file_list.clear();
        tinydir_dir dir;
        if (tinydir_open_sorted(&dir, resolved_path) == -1) {
            cout << "Error opening file" << endl;
            return;
        }
        for (int i = 0; i < dir.n_files; i++) {
            tinydir_file file;
            if (tinydir_readfile_n(&dir, &file, i) == -1) {
                cout << "Error getting file" << endl;
                return;
            }

            if(strcmp(file.name,".")==0 || (file.name[0]=='.' && file.name[1]!='.'))
                continue;
            OGUIFile *oguifile= new OGUIFile(_window, file.name, file.path, file.is_dir, file.extension, 0);
            _file_list.push_back(oguifile);

           // printf("\n");

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
            return _file_browser_result->_path;
        }else{
            return "";
        }

    }

    void OGUIFileBrowser::draw() {
        // ToDo (damiles): Create scroll for more files in window
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
            drawHeader(vg, "System", 0, 40, 200);

            // Draw system files
            int x=10;
            int y=75;
            for(int i=0; i<_system.size(); i++){
                OGUIFile *file= _system.at(i);
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

            }



            // Draw files
            x=210;
            y=50;
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

            // Show icon in header
            nvgFontSize(vg, 16.0f);
            nvgFontFace(vg, "icons");
            nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
            nvgFillColor(vg, nvgRGBA(0,0,0,255));
            nvgText(vg, 10, 11, "\uF115", NULL);
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
            nvgText(vg, 11, 13, "\uF115", NULL);
            nvgFillColor(vg, nvgRGBA(255,255,255,255));

            // show path in header
            nvgFontSize(vg, 18.0f);
            nvgFontFace(vg, "sans-bold");
            nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
            nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
            nvgText(vg, 35, 10, resolved_path, NULL);
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
            nvgText(vg, 36, 12, resolved_path, NULL);

            if(_file_browser_result!=NULL) {
                if (_file_browser_result->_is_dir == false) {
                    // show file in footer
                    nvgFontSize(vg, 18.0f);
                    nvgFontFace(vg, "sans-bold");
                    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_TOP);
                    nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
                    nvgText(vg, 209, _window->getWindowHeight()-32, _file_browser_result->_file_name.c_str(), NULL);
                    nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
                    nvgText(vg, 210, _window->getWindowHeight()-30, _file_browser_result->_file_name.c_str(), NULL);

                    // Draw preview if its image
                    if(_file_browser_result->isImage()){
                        x=0;
                        y=_window->getWindowHeight()-200;

                        drawHeader(vg, "Preview", 0, y-30, 200);


                        int w=200;
                        int h=200;
                        NVGpaint bg;
                        // Box
                        bg = nvgBoxGradient(vg, x+1,y+1+1.5f, w-2,h-2, 3,4, nvgRGBA(255,255,255,32), nvgRGBA(32,32,32,32));
                        nvgBeginPath(vg);
                        nvgRoundedRect(vg, x+1,y+1, w-2,h-2, 4-1);
                        nvgFillPaint(vg, bg);
                        nvgFill(vg);

                        nvgBeginPath(vg);
                        nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, 4-0.5f);
                        nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
                        nvgStroke(vg);

                        // Image preview
                        if(_preview_image==-1){
                            _preview_image= nvgCreateImageRGBA(vg, 150, 150, 0, _file_browser_result->getPreview());
                        }else{
                            nvgUpdateImage(vg, _preview_image, _file_browser_result->getPreview());
                        }

                        NVGpaint imgPaint = nvgImagePattern(vg, 25, y+25, 150, 150, 0, _preview_image, 1);
                        nvgBeginPath(vg);
                        nvgRect(vg, 25, y+25, 150,150);
                        nvgFillPaint(vg, imgPaint);
                        nvgFill(vg);
                    }


                }
            }


        }
    }
}