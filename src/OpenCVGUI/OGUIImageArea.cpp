#include <opencv2/imgproc/types_c.h>
#include "OGUIImageArea.h"
#include "OGUIWindow.h"

namespace OpenCVGUI {


OGUIImageArea::OGUIImageArea(OGUIWindow* window): OGUIArea(window)
{
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
        int imgw, imgh;
        nvgImageSize(vg, image, &imgw, &imgh);
        NVGpaint imgPaint = nvgImagePattern(vg, x+((width/2)-(imgw/2)), y, imgw, imgh, 0, image, 1);
        nvgBeginPath(vg);
        nvgRect(vg, x+((width/2)-(imgw/2)),y, imgw,imgh);
        nvgFillPaint(vg, imgPaint);
        nvgFill(vg);
    }

    nvgResetScissor(vg);
}

    void OGUIImageArea::setImage(Mat  img){
        NVGcontext* ctx= (window->vg);
        int w,h;

        w= img.cols;
        h= img.rows;
        // convert img to RGBA

        cvtColor(img, imgRGBA, CV_RGB2RGBA);
        //imshow("test", imgRGBA);
        data= imgRGBA.data;
        if(image==-1){
            image= nvgCreateImageRGBA(ctx, w, h, 0, data);
            //image= nvgCreateImage(ctx, "../resources/image1.jpg", 0);
        }else{
            nvgUpdateImage(ctx, image, data);
        }
    }

} /* End of namespace OpenCVGUI */
