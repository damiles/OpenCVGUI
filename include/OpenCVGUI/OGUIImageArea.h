#ifndef OpenCVGUI_OGUIImageArea_h
#define OpenCVGUI_OGUIImageArea_h

#include <string>
#include "OGUIArea.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <functional>

using namespace std;
using namespace cv;

namespace OpenCVGUI {

class OGUIImageArea : public OGUIArea {

 public:

    virtual void draw(int x, int y, int width, int height);
    virtual void updateScrollStatus(double xoffset,double yoffset);
    OGUIImageArea(OGUIWindow* window, string title="");
    void setImage(Mat *img);
    void setMouseClickCallBack(std::function<void(int, int)> func);
    void calcHistogram();
private:
    int image=-1;
    Mat _img, imgRGBA;
    unsigned char* data;
    float image_x, image_y, image_scale, image_width, image_height;
    float dx, dy; // mouse increment if it is moved.
    int last_mouse_x, last_mouse_y;
    bool has_to_update;
    string _title;

    void updateImage();

    int _prev_mouse_status;
    std::function<void(int, int)> btn_click_callback;

    int _count_delay;

    bool _show_info;

    Mat b_hist, g_hist, r_hist;

};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIImageArea_h
