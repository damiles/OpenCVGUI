//
// Created by damiles on 24/11/16.
//

#include "nanovg.h"
#include "OGUIFile.h"
#include "OGUIWindow.h"

namespace OpenCVGUI {

    OGUIFile::OGUIFile(void* window,const char* file_name, const char* path, int isdir, const char* ext, double size) {
        _file_name= file_name;
        _path= path;
        _ext= ext;
        _size= size;
        _is_dir= (bool)isdir;
        _window= window;
        _mouse_state=0;
    }

    bool OGUIFile::draw(void *vvg, int x, int y, int mouse_x, int mouse_y) {
        NVGcontext *vg = (NVGcontext *) vvg;
        OGUIWindow *window= (OGUIWindow *)_window;
        int width=200;
        int height= 20;

        nvgScissor(vg, x, y, width, height);

        bool internal_state=false;

        nvgBeginPath(vg);
        nvgRect(vg, x, y, width, height);
        if(mouse_x > x &&
           mouse_x < x+width &&
           mouse_y > y &&
           mouse_y < y + height
                ) {

            nvgFillColor(vg, nvgRGBA(50,53,58,255));
            window->setCursor(HAND_CURSOR);
            if(window->mouse_state == 1){
                _mouse_state=1;
            }else if(window->mouse_state == 0){
                if(_mouse_state==1) {
                    internal_state = true;
                    _mouse_state = 0;
                }
            }

        }else {
            nvgFillColor(vg, nvgRGBA(50,53,58,180));
            window->setCursor(DEFAULT_CURSOR);
        }
        nvgFill(vg);

        // Draw text
        nvgFontSize(vg, 13.0f);
        nvgFontFace(vg, "icons");
        nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
        nvgFillColor(vg, nvgRGBA(0,0,0,255));
        if(_is_dir) {
            nvgText(vg, x+5, y+3, "\uF115", NULL);
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
            nvgText(vg, x + 6, y + 5, "\uF115", NULL);
            nvgFillColor(vg, nvgRGBA(255,255,255,255));
        }else{
            nvgText(vg, x+5, y+3, "\uF016", NULL);
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
            nvgText(vg, x + 6, y + 5, "\uF016", NULL);
            nvgFillColor(vg, nvgRGBA(255,255,255,255));
        }

        nvgFontSize(vg, 15.0f);
        nvgFontFace(vg, "sans");
        nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
        nvgFillColor(vg, nvgRGBA(0,0,0,255));
        nvgText(vg, x+24, y+1, _file_name.c_str() , NULL);
        nvgFillColor(vg, nvgRGBA(255,255,255,255));
        nvgText(vg, x+25, y+3,  _file_name.c_str(), NULL);

        nvgResetScissor(vg);

        return internal_state;
    }
}