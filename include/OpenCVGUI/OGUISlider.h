#ifndef OpenCVGUI_OGUISlider_h
#define OpenCVGUI_OGUISlider_h

#include <vector>
#include "OGUIWidget.h"
#include <functional>

namespace OpenCVGUI {

class OGUISlider : public OGUIWidget {

 public:

    virtual void draw(int x, int y, int width);

    OGUISlider(const char* title,double min,double max, double value);
    void setCallBack(std::function<void(double)> func);
    void setValue(double v);
private:
    double min, max, value;
    const char* title;
    int actual_press_status;
    std::function<void(double)> btn_click_callback;
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUISlider_h
