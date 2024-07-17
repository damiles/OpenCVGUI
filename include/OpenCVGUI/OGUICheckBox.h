#ifndef OpenCVGUI_OGUICheckBox_h
#define OpenCVGUI_OGUICheckBox_h

#include "OGUIWidget.h"
#include <functional>

namespace OpenCVGUI {

class OGUICheckBox : public OGUIWidget {

 public:

    virtual void draw(int x, int y, int width);

    OGUICheckBox(const char* title, bool value);
    void setCallBack(std::function<void(bool)> func);
 private:
    const char* title;
    bool value;
    int actual_press_status;
    std::function<void(bool)> btn_click_callback;
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUICheckBox_h
