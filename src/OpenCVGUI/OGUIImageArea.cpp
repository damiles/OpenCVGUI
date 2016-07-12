#include <opencv2/imgproc/types_c.h>
#include "OGUIImageArea.h"
#include "OGUIWindow.h"

namespace OpenCVGUI {


OGUIImageArea::OGUIImageArea(OGUIWindow* window): OGUIArea(window)
{
    image_scale=1.0;
}


void OGUIImageArea::draw(int x, int y, int width, int height)
{
    this->x=x;
    this->y=y;
    this->width= width;
    this->height= height;
    NVGcontext* vg= (window->vg);

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

    nvgResetScissor(vg);
}

    void OGUIImageArea::setImage(Mat  img){
        NVGcontext* ctx= (window->vg);

        image_width= img.cols;
        image_height= img.rows;
        // convert img to RGBA

        cvtColor(img, imgRGBA, CV_BGR2RGBA);
        data= imgRGBA.data;
        if(image==-1){
            image= nvgCreateImageRGBA(ctx, image_width, image_height, 0, data);
        }else{
            nvgUpdateImage(ctx, image, data);
        }
    }

} /* End of namespace OpenCVGUI */
