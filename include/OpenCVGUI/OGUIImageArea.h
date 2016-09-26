#ifndef OpenCVGUI_OGUIImageArea_h
#define OpenCVGUI_OGUIImageArea_h

#include "OGUIArea.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

namespace OpenCVGUI {

class OGUIImageArea : public OGUIArea {

 public:

    virtual void draw(int x, int y, int width, int height);
    virtual void updateScrollStatus(double xoffset,double yoffset);
    OGUIImageArea(OGUIWindow* window, string title="");
    void setImage(Mat  img);

private:
    int image=-1;
    Mat _img, imgRGBA;
    unsigned char* data;
    float image_x, image_y, image_scale, image_width, image_height;
    bool has_to_update;
    string _title;

    void updateImage();



};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIImageArea_h
