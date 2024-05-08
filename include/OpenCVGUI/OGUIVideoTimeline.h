#ifndef OpenCVGUI_OGUIVideoTimeline_h
#define OpenCVGUI_OGUIVideoTimeline_h

#include <opencv2/core/core.hpp>
#include <opencv2/videoio.hpp>
#include "OGUIArea.h"
#include "nanovg.h"

using namespace cv;
using namespace std;

namespace OpenCVGUI {

    class TimeMarker {
        public:
            int start; // in ms
            int end;
            string title;
            NVGcolor color;
            TimeMarker(string title, int start, int end, NVGcolor color): title(title), start(start), end(end), color(color) {};
            void draw(void* vvg, int x, int y, int mouse_x, int mouse_y);
    };

    class Track {
        public:
            Track(string title, VideoCapture* videocap);
            Mat thumbnail;
            VideoCapture* videocap;
            double duration;
            string title;
            void draw(void* vvg, int x, int y, int width, int height, int mouse_x, int mouse_y);
            void add_marker(string title, int start, int end, NVGcolor color);
        private:
            vector<TimeMarker> markers;
            int nvg_thumbnail_id;
            const unsigned char* thumb_data;
    };

    class OGUIVideoTimeline: public OGUIArea {
        public:
            void draw(int x, int y, int width, int height);
            OGUIVideoTimeline(OGUIWindow* window, std::string title);
            void updateScrollStatus(double xoffset,double yoffset);
            void addTrack(Track *track);
            int get_time_pos();
            void set_time_pos(int pos_ms);
            bool is_playing();
        private:
            bool _playing;
            int time_pos; // time position in px
            int time_pos_msec;
            vector<Track*> tracks;
            string format_secs(int secs);

    };

    
}
#endif // OpenCVGUI_OGUIVideoTimeline_h