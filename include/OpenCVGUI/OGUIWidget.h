#ifndef OpenCVGUI_OGUIWidget_h
#define OpenCVGUI_OGUIWidget_h

namespace OpenCVGUI {
class OGUIFormArea;
} /* End of namespace OpenCVGUI */

namespace OpenCVGUI {

class OGUIWidget {

 public:

    virtual void draw();

 public:

    OGUIFormArea *area;
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIWidget_h
