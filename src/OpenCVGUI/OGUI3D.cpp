#include <GL/glew.h>
#include "OGUI3D.h"
#include <iostream>
#include <nanovg.h>
#include "OGUIWindow.h"

namespace OpenCVGUI {


void OGUI3D::CreateVertexBuffer()
{
    Vector3f Vertices[1];
    Vertices[0] = Vector3f(0.0f, 0.0f, 0.0f);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

void OGUI3D::draw(int x, int y, int width, int height)
{
    this->x=x;
    this->y=y;
    this->width= width;
    this->height= height;
    NVGcontext* vg= (NVGcontext*)(window->vg);

    nvgScissor(vg, x, y, width, height);
    OGUIArea::draw(x,y,width,height);

    if(isMouseIn()) {
        nvgBeginPath(vg);
        nvgRect(vg, x, y, width, 22);
        nvgFillColor(vg, nvgRGBA(0, 0, 0, 100));
        nvgFill(vg);

        // Draw text
        nvgFontSize(vg, 16.0f);
        nvgFontFace(vg, "sans");
        float tw = nvgTextBounds(vg, 0, 0, title.c_str(), NULL, NULL);
        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
        nvgText(vg, x + width * 0.5f - tw * 0.5f, y + 11, title.c_str(), NULL);
    }
    nvgResetScissor(vg);
}

void OGUI3D::draw3d(int x, int y, int width, int height)
{
    // ToDo Enable GL3 version...

    glViewport(x, y, (GLsizei)(width), (GLsizei)(height));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_POINTS, 0, 1);

    glDisableVertexAttribArray(0);

}

OGUI3D::OGUI3D(OGUIWindow* window, std::string title): OGUIArea(window)
{
    this->type= 1; // 3d type window
    this->title= title;
    CreateVertexBuffer();
}

} /* End of namespace OpenCVGUI */
