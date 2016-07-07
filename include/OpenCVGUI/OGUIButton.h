#ifndef OpenCVGUI_OGUIButton_h
#define OpenCVGUI_OGUIButton_h

#include "OGUIWidget.h"


namespace OpenCVGUI {

class OGUIButton : public OGUIWidget {

 public:

    virtual void draw(int x, int y, int width, int height);
    OGUIButton(const char* title);

private:
    const char* title;
    int actual_press_status;
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIButton_h
