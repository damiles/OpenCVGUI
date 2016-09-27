#include <GL/glew.h>
#include "OGUI3D.h"

namespace OpenCVGUI {




void OGUI3D::draw(int x, int y, int width, int height)
{
    glBegin(GL_POINTS);                      // Select points as the primitive
    glVertex3f(0.0f, 0.0f, 0.0f);    // Specify a point
    glVertex3f(50.0f, 50.0f, 50.0f); // Specify another point
    glEnd();
}

OGUI3D::OGUI3D(OGUIWindow* window, std::string title): OGUIArea(window)
{
    this->title= title;
}

} /* End of namespace OpenCVGUI */
