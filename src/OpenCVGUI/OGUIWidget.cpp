#include "OGUIWidget.h"

namespace OpenCVGUI {




void OGUIWidget::draw(int x, int y, int width, int height)
{
    this->x= x;
    this->y= y;
    this->width= width;
    this->height= height;
}

} /* End of namespace OpenCVGUI */
