#include <opencv2/imgproc/types_c.h>
#include "OGUIImageArea.h"
#include "OGUIWindow.h"
#include "nanovg.h"

namespace OpenCVGUI {


    OGUIImageArea::OGUIImageArea(OGUIWindow* window, string title): OGUIArea(window)
    {
        image_scale=1.0;
        has_to_update= false;
        _title= title;
    }

    void OGUIImageArea::updateScrollStatus(double xoffset,double yoffset)
    {
        if(isMouseIn())
            image_scale+= (yoffset/20);
    }

    void OGUIImageArea::draw(int x, int y, int width, int height)
    {
        this->x=x;
        this->y=y;
        this->width= width;
        this->height= height;
        NVGcontext* vg= (NVGcontext*)(window->vg);

        nvgScissor(vg, x, y, width, height);
        OGUIArea::draw(x,y,width,height);

        for(int i=0; i<width-2; i+=40){
            for(int j=0; j<height-2; j+=20){
                nvgBeginPath(vg);
                int offset=20;
                if((j%40)==0)
                    offset=0;
                int xs= x+i+offset+1;
                int ys= y+j+1;
                int w=20;
                int h=20;
                if(xs+20 > x+width-2)
                    w= x+width-2-xs;
                if(ys+20 > y+height-2)
                    h= y+height-2-ys;
                nvgRect(vg, xs, ys, w, h);
                nvgFillColor(vg, nvgRGBA(50,50,50,100));
                nvgFill(vg);
            }
        }

        updateImage();
        if(image!=-1){
            float imgw= image_width*image_scale;
            float imgh= image_height*image_scale;
            image_x=x+((width/2)-(imgw/2));
            image_y=y+((height/2)-(imgh/2));
            NVGpaint imgPaint = nvgImagePattern(vg, image_x, image_y, imgw, imgh, 0, image, 1);
            nvgBeginPath(vg);
            nvgRect(vg, image_x, image_y, imgw,imgh);
            nvgFillPaint(vg, imgPaint);
            nvgFill(vg);
        }

        if(isMouseIn()) {
            nvgBeginPath(vg);
            nvgRect(vg, x, y, width, 22);
            nvgFillColor(vg, nvgRGBA(0, 0, 0, 100));
            nvgFill(vg);

            // Draw text
            nvgFontSize(vg, 16.0f);
            nvgFontFace(vg, "sans");
            float tw = nvgTextBounds(vg, 0, 0, _title.c_str(), NULL, NULL);
            nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
            nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
            nvgText(vg, x + width * 0.5f - tw * 0.5f, y + 11, _title.c_str(), NULL);
        }
        nvgResetScissor(vg);
    }

    void OGUIImageArea::updateImage(){

        if(has_to_update){
            NVGcontext *ctx = (NVGcontext*)(window->vg);
            if(image==-1){
                image= nvgCreateImageRGBA(ctx, image_width, image_height, 0, data);
            }else{
                nvgUpdateImage(ctx, image, data);
            }
            has_to_update= false;
        }
    }

    void OGUIImageArea::setImage(Mat  img) {

        img.copyTo(_img);

        // Reinit image if don't have same size
        if(img.cols!= image_width || img.rows!= image_height)
            image==-1;

        image_width = img.cols;
        image_height = img.rows;
        // convert img to RGBA
        switch (img.channels())
        {
            case 1:
                cvtColor(_img, imgRGBA, CV_GRAY2RGBA);
                data = imgRGBA.data;
                break;
            case 3:
                cvtColor(_img, imgRGBA, CV_BGR2RGBA);
                data = imgRGBA.data;
                break;
            case 4:
                data = _img.data;
                break;
        }
        has_to_update= true;
    }

} /* End of namespace OpenCVGUI */
