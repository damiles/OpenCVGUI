#include "OGUIVideoTimeline.h"
#include "OGUIWindow.h"
#include "OGUIUtils.h"
#include "nanovg.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <GLFW/glfw3.h>

namespace OpenCVGUI {

    OGUIVideoTimeline::OGUIVideoTimeline(OGUIWindow* window, std::string title): OGUIArea(window)
    {
        this->title= title;      
        time_pos = 0;  
        time_pos_msec = 0;
        _playing= false;
    }

    string OGUIVideoTimeline::format_secs(int sec) {
        int min = sec / 60;
        int hours = min / 60;
        
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(2) << hours;
        ss << ":";
        ss << std::setfill('0') << std::setw(2) << min%60;
        ss << ":";
        ss << std::setfill('0') << std::setw(2) << sec%60;
        return ss.str();
    }

    bool OGUIVideoTimeline::is_playing()
    {
        return _playing;
    }

    void OGUIVideoTimeline::draw(int x, int y, int width, int height)
    {
        this->x=x;
        this->y=y;
        this->width= width;
        this->height= height;
        NVGcontext* vg= (NVGcontext*)(window->vg);
        nvgScissor(vg, x, y, width, height);
        OGUIArea::draw(x,y,width,height);

        drawTitle(true);

        int offset_time=150;
        

        // Draw tracks
        for(int i=0; i<tracks.size(); i++){
            Track* track= tracks[i];
            track->draw(vg, x, y+43+i*40, width, 40, this->window->mouse_x, this->window->mouse_y);
        }

        if(isMouseIn() && this->window->mouse_state == GLFW_PRESS){
            // check if is in the header track part
            if(this->window->mouse_x > x+offset_time && this->window->mouse_y > y+22 && this->window->mouse_y < y+height)
                time_pos = this->window->mouse_x - x - offset_time;
                time_pos_msec = time_pos*100;
        }

        // Draw header time
        nvgBeginPath(vg);
        nvgRect(vg, x+offset_time, y + 22, width-offset_time, 20);
        nvgFillColor(vg, nvgRGBA(0, 0, 0, 50));
        nvgFill(vg);

        // Draw time dragger
        nvgBeginPath(vg);
        nvgRect(vg, x + offset_time + time_pos, y+22, 1, height);
        nvgFillColor(vg, nvgRGBA(255,255,255, 255));
        nvgFill(vg);
        
        nvgBeginPath(vg);
        nvgMoveTo(vg, x + offset_time - 5 + time_pos, y + 22);
        nvgLineTo(vg, x + offset_time + time_pos, y + 22 + 5);
        nvgLineTo(vg, x + offset_time + 5 + time_pos, y + 22);
        nvgFillColor(vg, nvgRGBA(255,255,255, 255));
        nvgFill(vg);


        // Draw ticks
        for(int i=offset_time; i<width; i=i+25){
            int t= (i-offset_time)%100;
            nvgBeginPath(vg);
            if(t==0)
                nvgRect(vg, x + i, y+28, 1, 14);
            else
                nvgRect(vg, x + i, y+37, 1, 5);
            nvgFillColor(vg, nvgRGBA(255,255,255, 125));
            nvgFill(vg);
        }
        // Draw time texts
        for(int i=offset_time; i<width; i=i+100){
            int start_time = i-offset_time;
            int sec=start_time/10;
            drawLabel(vg, format_secs(sec).c_str(), x + i + 2, y+32, "sans", 12.0f, 255,255,255,125 );
        }

        // draw right header tracks
        nvgBeginPath(vg);
        nvgRect(vg, x, y + 22, offset_time - 1, 20);
        nvgFillColor(vg, nvgRGBA(0, 0, 0, 50));
        nvgFill(vg);

        if(drawBasicButton(vg, this->window, "\uF04b", x+2, y + 22 , 20, 20, this->window->mouse_x, this->window->mouse_y, "icons", 16)){
                _playing= true;
        }

        if(drawBasicButton(vg, this->window, "\uF04c", x+24, y + 22 , 20, 20, this->window->mouse_x, this->window->mouse_y, "icons", 16)){
                _playing= false;
        }

        // draw actual time
        int act_secs = time_pos_msec/1000; 
        string actuall_time_str = format_secs(act_secs);
        drawLabel(vg, actuall_time_str.c_str(), x + 52, y+32, "sans", 16.0f, 255,255,255,255 );

        // draw right header tracks
        nvgBeginPath(vg);
        nvgRect(vg, x, y + 43, offset_time - 1, height - 21);
        nvgFillColor(vg, nvgRGBA(0, 0, 0, 50));
        nvgFill(vg);
        
        
    }
    
    void OGUIVideoTimeline::updateScrollStatus(double xoffset,double yoffset)
    {
        if(isMouseIn()){

        }
    }

    void OGUIVideoTimeline::addTrack(Track *track)
    {
        tracks.push_back(track);
    }

    int OGUIVideoTimeline::get_time_pos()
    {
        return time_pos_msec;
    }

    void OGUIVideoTimeline::set_time_pos(int pos_ms)
    {
        time_pos_msec = pos_ms;
        time_pos = pos_ms/100;
    }


    Track::Track(string title, VideoCapture* videocap)
    {
        this->title = title;
        this->videocap = videocap;
        double fps = this->videocap->get(CAP_PROP_FPS);
        double frame_count = this->videocap->get(CAP_PROP_FRAME_COUNT);
        duration = frame_count/fps;
        this->videocap->set(CAP_PROP_POS_MSEC, 0);
        this->videocap->read(this->thumbnail);
        // this->thumbnail = thumbnail;
        cvtColor(thumbnail, thumbnail, COLOR_BGR2RGBA);
        nvg_thumbnail_id=-1;
    }

    void Track::draw(void* vvg, int x, int y, int width, int height, int mouse_x, int mouse_y)
    {
        NVGcontext* vg= (NVGcontext*)(vvg);
        const int offset_time=150;
        nvgBeginPath(vg);
        nvgRect(vg, x + offset_time, y, width-offset_time, 38);
        nvgFillColor(vg, nvgRGBA(0, 0, 0, 20));
        nvgFill(vg);

        nvgBeginPath(vg);
        nvgRect(vg, x, y + 39, width, 1);
        nvgFillColor(vg, nvgRGBA(255,255,255, 20));
        nvgFill(vg);

        // Draw box of duration time size
        nvgBeginPath(vg);
        nvgRect(vg, x + offset_time, y, duration*10, 38);
        nvgFillColor(vg, nvgRGBA(255, 0, 0, 20));
        nvgFill(vg);
        
        // Draw thumbnail
        int w=thumbnail.cols*40/thumbnail.rows;
        int h=40;
        if (nvg_thumbnail_id==-1){
            thumb_data= thumbnail.data;
            nvg_thumbnail_id= nvgCreateImageRGBA(vg, thumbnail.cols, thumbnail.rows, NVG_IMAGE_NEAREST, thumb_data);
            // nvgUpdateImage(vg, nvg_thumbnail_id, thumbnail.data);
        }
        
        
        NVGpaint imgPaint = nvgImagePattern(vg, x+offset_time, y, w, h, 0, nvg_thumbnail_id, 1);
        nvgBeginPath(vg);
        nvgRect(vg, x+offset_time, y, w, h);
        nvgFillPaint(vg, imgPaint);
        nvgFill(vg);

        drawLabel(vg, title.c_str(), x + 10, y+20, "sans", 16.0f, 255,255,255,255 );

        for(auto m:markers){
            m.draw(vg, x+offset_time, y, mouse_x, mouse_y);
        }

    }

    void Track::add_marker(string title, int start, int end, NVGcolor color)
    {
        markers.push_back(TimeMarker(title, start, end, color));
    }

    void TimeMarker::draw(void* vvg, int x, int y, int mouse_x, int mouse_y)
    {
        NVGcontext* vg= (NVGcontext*)(vvg);
        nvgBeginPath(vg);
        int w= (end-start)/100;
        int s = x + start/100;
        nvgRect(vg, s, y, w, 5);
        nvgFillColor(vg, color);
        nvgFill(vg);

        if(mouse_x > s && mouse_x< s+w && mouse_y > y && mouse_y < y+5)
        {
            nvgBeginPath(vg);
            nvgRect(vg, mouse_x+12, mouse_y+12, title.length()*8, 20);
            nvgFillColor(vg, nvgRGBA(0, 0, 0, 200));
            nvgFill(vg);

            drawLabel(vg, title.c_str(), mouse_x+16, mouse_y+22, "sans", 16.0f, 255,255,255,255 );
        }
    }

}