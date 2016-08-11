#include "OGUIWidget.h"

namespace OpenCVGUI {




void OGUIWidget::draw(int x, int y, int width)
{
    this->x= x;
    this->y= y;
    this->width= width;
}

void OGUIWidget::characterCallback(unsigned int key) {

}

void OGUIWidget::keyCallback(int key, int scancode, int action, int mods) {

}

} /* End of namespace OpenCVGUI */
