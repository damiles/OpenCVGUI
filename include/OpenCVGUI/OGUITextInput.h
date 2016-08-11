#ifndef OpenCVGUI_OGUITextInput_h
#define OpenCVGUI_OGUITextInput_h

#include "OGUIWidget.h"
#include <string>

using namespace std;

namespace OpenCVGUI {

class OGUITextInput : public OGUIWidget {

 public:

    virtual void draw(int x, int y, int w);
    virtual void characterCallback(unsigned int key);
    virtual void keyCallback(int key, int scancode, int action, int mods);

    string value;
    bool is_focus_;
    int animation_alpha_;
    const char* label;
    OGUITextInput(const char* label);
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUITextInput_h
