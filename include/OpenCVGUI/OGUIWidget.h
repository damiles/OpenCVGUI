#ifndef OpenCVGUI_OGUIWidget_h
#define OpenCVGUI_OGUIWidget_h

namespace OpenCVGUI {
class OGUIFormArea;
} /* End of namespace OpenCVGUI */

namespace OpenCVGUI {

class OGUIWidget {

 public:

    virtual void draw(int x, int y, int widht, int height);
    OGUIFormArea *area;

private:
    int x, y, width, height;
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUIWidget_h
