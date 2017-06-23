#include <opencv2/imgproc/types_c.h>
#include "OGUIImageArea.h"
#include "OGUIWindow.h"
#include <GLFW/glfw3.h>
#include "nanovg.h"
#include "OGUIUtils.h"

namespace OpenCVGUI {


    OGUIImageArea::OGUIImageArea(OGUIWindow* window, string title): OGUIArea(window)
    {
        image_scale=1.0;
        has_to_update= false;
        this->title= title;
        _prev_mouse_status= GLFW_RELEASE;
        last_mouse_x=-1;
        last_mouse_y=-1;
        dx=0; dy=0;
        _count_delay=0;
        _show_info= false;
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
        float imgw= 0;
        float imgh= 0;
        if(image!=-1){
            imgw= image_width*image_scale;
            imgh= image_height*image_scale;
            image_x=x+((width/2)-(imgw/2)) + dx;
            image_y=y+((height/2)-(imgh/2)) + dy;
            NVGpaint imgPaint = nvgImagePattern(vg, image_x, image_y, imgw, imgh, 0, image, 1);
            nvgBeginPath(vg);
            nvgRect(vg, image_x, image_y, imgw,imgh);
            nvgFillPaint(vg, imgPaint);
            nvgFill(vg);
        }

        drawTitle();

        if(isMouseIn() || _show_info) {
            int mouse_x_img= (this->window->mouse_x - image_x) * (1.0/image_scale);
            int mouse_y_img= (this->window->mouse_y - image_y) * (1.0/image_scale);

            // Draw X,Y mouse position
            if(mouse_x_img >= 0 && mouse_x_img < image_width &&
                    mouse_y_img >= 0 && mouse_y_img < image_height) {

                // Show mouse position in image
                stringstream mouse_pos;
                mouse_pos << "x: " << mouse_x_img << ", y:" << mouse_y_img;
                float tw = nvgTextBounds(vg, 0, 0, mouse_pos.str().c_str(), NULL, NULL);
                nvgFontSize(vg, 16.0f);
                nvgFontFace(vg, "sans");
                nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
                nvgText(vg, x + 10.5f, y + 11, mouse_pos.str().c_str(), NULL);

                // show color value
                if (_img.channels()==1) {
                    stringstream gray_val;
                    gray_val << "Gray value: " << (int)_img.at<uchar>(mouse_y_img, mouse_x_img, 0);
                    nvgFontSize(vg, 16.0f);
                    nvgFontFace(vg, "sans");
                    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                    nvgFillColor(vg, nvgRGBA(255, 255, 255, 180));
                    nvgText(vg, x + 10.5f + tw + 10.5f, y + 11, gray_val.str().c_str(), NULL);
                }else if(_img.channels()==3){
                    stringstream color_val;
                    color_val << "R: " << (int)_img.at<Vec3b>(mouse_y_img, mouse_x_img)[2];
                    nvgFontSize(vg, 16.0f);
                    nvgFontFace(vg, "sans");
                    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                    nvgFillColor(vg, nvgRGBA(255, 0, 0, 255));
                    nvgText(vg, x + 10.5f + tw + 10.5f, y + 11, color_val.str().c_str(), NULL);

                    tw = tw + nvgTextBounds(vg, 0, 0, color_val.str().c_str(), NULL, NULL) + 2.5f;
                    color_val.str("");
                    color_val << "G: " << (int)_img.at<Vec3b>(mouse_y_img, mouse_x_img)[1];
                    nvgFontSize(vg, 16.0f);
                    nvgFontFace(vg, "sans");
                    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                    nvgFillColor(vg, nvgRGBA(0, 255, 0, 255));
                    nvgText(vg, x + 10.5f + tw + 10.5f, y + 11, color_val.str().c_str(), NULL);

                    tw = tw + nvgTextBounds(vg, 0, 0, color_val.str().c_str(), NULL, NULL) + 2.5f;
                    color_val.str("");
                    color_val << "B: " << (int)_img.at<Vec3b>(mouse_y_img, mouse_x_img)[0];
                    nvgFontSize(vg, 16.0f);
                    nvgFontFace(vg, "sans");
                    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
                    nvgFillColor(vg, nvgRGBA(0, 0, 255, 255));
                    nvgText(vg, x + 10.5f + tw + 10.5f, y + 11, color_val.str().c_str(), NULL);

                }

            }

            if(drawBasicButton(vg, this->window, "\uF05a", x+width-60, y, 30, 22, this->window->mouse_x, this->window->mouse_y, "icons", 16)){
                _show_info=true;
            }

            /// Click event test
            if(this->window->mouse_state == GLFW_PRESS && this->window->key_pressed == GLFW_KEY_LEFT_CONTROL){

                if(last_mouse_x==-1){
                    last_mouse_x= this->window->mouse_x;
                    last_mouse_y= this->window->mouse_y;
                }

                dx+= (this->window->mouse_x-last_mouse_x);
                dy+= (this->window->mouse_y-last_mouse_y);

                last_mouse_x= this->window->mouse_x;
                last_mouse_y= this->window->mouse_y;

            }else if (this->window->mouse_state == GLFW_PRESS && _prev_mouse_status == GLFW_RELEASE){
                _prev_mouse_status = GLFW_PRESS;
            }else if(this->window->mouse_state == GLFW_RELEASE && _prev_mouse_status == GLFW_PRESS){
                _prev_mouse_status = GLFW_RELEASE;
                if(btn_click_callback!=NULL) {
                    btn_click_callback(mouse_x_img, mouse_y_img);
                }
            }

            if(this->window->mouse_state == GLFW_RELEASE){
                last_mouse_x= -1;
                last_mouse_y= -1;
            }
        }// End mouse in


        if(_show_info){
            float cornerRadius = 3.0f;
            NVGpaint shadowPaint;
            int w=256;
            int h=300;
            float arrx = 230.5f;
            int sx= x + width - w -20;
            int sy= y + 32;
            shadowPaint = nvgBoxGradient(vg, sx,sy+4, w,h, cornerRadius*2, 20, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
            nvgBeginPath(vg);
            nvgRect(vg, sx-10,sy-10, w+20,h+30);
            nvgRoundedRect(vg, sx,sy, w,h, cornerRadius);
            nvgPathWinding(vg, NVG_HOLE);
            nvgFillPaint(vg, shadowPaint);
            nvgFill(vg);

            // Window
            nvgBeginPath(vg);
            nvgRoundedRect(vg, sx,sy, w,h, cornerRadius);
            nvgMoveTo(vg, sx + arrx ,sy - 10);
            nvgLineTo(vg, sx + arrx - 11,sy + 1);
            nvgLineTo(vg, sx + arrx + 11,sy + 1);
            nvgFillColor(vg, nvgRGBA(68,70,74,255));
            nvgFill(vg);

            drawHeader( vg, "Properties", sx, sy, w);
            stringstream t;
            t << "Width: " << _img.cols;
            drawLabel( vg, t.str().c_str(), sx + 20, sy + 50);
            t.str("");
            t << "Height: " << _img.rows;
            drawLabel( vg, t.str().c_str(), sx + 128, sy + 50);
            t.str("");
            t << "Num channels: " << _img.channels();
            drawLabel( vg, t.str().c_str(), sx + 20, sy + 70);

            drawHeader( vg, "Histogram", sx, sy+100, w);

            nvgBeginPath(vg);
            nvgRoundedRect(vg, sx,sy+130, w,130, cornerRadius);
            nvgFillColor(vg, nvgRGBA(30,30,30,255));
            nvgFill(vg);

            if(b_hist.data!=NULL) {
                for (int i = 0; i <= 256; i++) {
                    float val = b_hist.at<float>(i);
                    nvgBeginPath(vg);
                    nvgRect(vg, sx+i, sy + 260 - val, 1, val);
                    if (_img.channels() == 1)
                        nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
                    else
                        nvgFillColor(vg, nvgRGBA(0, 0, 255, 128));
                    nvgFill(vg);

                    if (_img.channels() == 3) {
                        val = g_hist.at<float>(i);
                        nvgBeginPath(vg);
                        nvgRect(vg, sx+i, sy + 260 - val, 1, val);
                        nvgFillColor(vg, nvgRGBA(0, 255, 0, 128));
                        nvgFill(vg);

                        val = r_hist.at<float>(i);
                        nvgBeginPath(vg);
                        nvgRect(vg, sx+i, sy + 260 - val, 1, val);
                        nvgFillColor(vg, nvgRGBA(255, 0, 0, 128));
                        nvgFill(vg);
                    }
                }
            }

            if(drawBasicButton(vg, this->window, "Close", sx+20, sy + h - 30, 216, 22, this->window->mouse_x, this->window->mouse_y, "sans", 16)){
               _show_info= false;
            }
        }

        nvgResetScissor(vg);
    }

    void OGUIImageArea::calcHistogram(){
        /// Separate the image in 3 places ( B, G and R )
        vector<Mat> bgr_planes;
        split( _img, bgr_planes );

        /// Establish the number of bins
        int histSize = 256;

        /// Set the ranges ( for B,G,R) )
        float range[] = { 0, 256 } ;
        const float* histRange = { range };

        bool uniform = true; bool accumulate = false;

        double maxVal=0;
        /// Compute the histograms:
        calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
        minMaxLoc(b_hist, NULL, &maxVal);
        if(_img.channels()==3) {
            calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
            calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);
            double tmpMax;
            minMaxLoc(g_hist, NULL, &tmpMax);
            if(tmpMax>maxVal)
                maxVal= tmpMax;
            minMaxLoc(r_hist, NULL, &tmpMax);
            if(tmpMax>maxVal)
                maxVal= tmpMax;
        }

        // Normalize to max value
        b_hist.convertTo(b_hist, -1, 130/maxVal);
        if(_img.channels()==3) {
            r_hist.convertTo(r_hist, -1, 130/maxVal);
            g_hist.convertTo(g_hist, -1, 130/maxVal);
        }

    }

    void OGUIImageArea::updateImage(){

        if(has_to_update){
            NVGcontext *ctx = (NVGcontext*)(window->vg);
            if(image==-1){
                image= nvgCreateImageRGBA(ctx, image_width, image_height, NVG_IMAGE_NEAREST, data);
            }else{
                nvgUpdateImage(ctx, image, data);
            }
            //std::thread t1(&OGUIImageArea::calcHistogram, this);
            calcHistogram();
            has_to_update= false;
        }
    }

    void OGUIImageArea::setImage(Mat *img) {

        img->copyTo(_img);

        // Reinit image if don't have same size
        if(img->cols!= image_width || img->rows!= image_height)
            image=-1;

        image_width = img->cols;
        image_height = img->rows;
        // convert img to RGBA
        switch (img->channels())
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

    void OGUIImageArea::setMouseClickCallBack(std::function<void(int, int)> func) {
        btn_click_callback= func;
    }

} /* End of namespace OpenCVGUI */
