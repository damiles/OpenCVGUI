//
// Created by damiles on 24/11/16.
//

#include "nanovg.h"
#include "OGUIFile.h"
#include "OGUIWindow.h"

using namespace cv;

namespace OpenCVGUI {

    OGUIFile::OGUIFile(void* window,const char* file_name, const char* path, int isdir, const char* ext, double size) {
        _file_name= file_name;
        _path= path;
        _ext= ext;
        _size= size;
        _is_dir= isdir;
        _window= window;
        _mouse_state=0;
        _preview_data= NULL;
    }

    bool OGUIFile::draw(void *vvg, int x, int y, int mouse_x, int mouse_y) {
        NVGcontext *vg = (NVGcontext *) vvg;
        OGUIWindow *window= (OGUIWindow *)_window;
        int width=200;
        if(_is_dir==2){
            width=180;
        }
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
        if(_is_dir==1) {
            nvgText(vg, x+5, y+3, "\uF115", NULL);
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
            nvgText(vg, x + 6, y + 5, "\uF115", NULL);
            nvgFillColor(vg, nvgRGBA(255,255,255,255));
        }else if(_is_dir==2){
            nvgText(vg, x+5, y+3, "\uF0A0", NULL);
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
            nvgText(vg, x + 6, y + 5, "\uF0A0", NULL);
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
        //nvgFillColor(vg, nvgRGBA(0,0,0,255));
        //nvgText(vg, x+24, y+1, _file_name.c_str() , NULL);
        nvgFillColor(vg, nvgRGBA(255,255,255,255));
        nvgText(vg, x+25, y+3,  _file_name.c_str(), NULL);

        nvgResetScissor(vg);

        return internal_state;
    }

    bool OGUIFile::isImage() {
        if(_ext.compare("jpg")==0 ||
           _ext.compare("jpeg")==0 ||
           _ext.compare("png")==0 ||
           _ext.compare("tga")==0 ||
           _ext.compare("tiff")==0 ||
           _ext.compare("ppm")==0 ||
           _ext.compare("pgm")==0 ||
           _ext.compare("bmp")==0 ||
           _ext.compare("pxm")==0 ||
           _ext.compare("pnm")==0 ||
           _ext.compare("tif")==0 ||
           _ext.compare("exr")==0 ||
           _ext.compare("hdr")==0 ||
           _ext.compare("pic")==0 ||
           _ext.compare("jp2")==0 ||
           _ext.compare("jpe")==0 ||
           _ext.compare("dib")==0
                ){
            return true;
        } else{
            return false;
        }


    }

    unsigned char* OGUIFile::getPreview() {
        // generate data
        if(_preview_data==NULL){
            Mat img= imread(_path);
            Mat imgRGBA, _img;
            int image_width = img.cols;
            int image_height = img.rows;
            if(image_width < image_height)
                resize(img, _img, Size(150,image_height*150/image_width));
            else
                resize(img, _img, Size(image_width*150/image_height, 150));

            _img= _img(Rect(0,0,150,150));
            // convert img to RGBA
            switch (img.channels())
            {
                case 1:
                    cvtColor(_img, imgRGBA, CV_GRAY2RGBA);
                    _preview_data = imgRGBA.data;
                    break;
                case 3:
                    cvtColor(_img, imgRGBA, CV_BGR2RGBA);
                    _preview_data = imgRGBA.data;
                    break;
                case 4:
                    _preview_data = _img.data;
                    break;
            }
        }

        return _preview_data;

    }

}