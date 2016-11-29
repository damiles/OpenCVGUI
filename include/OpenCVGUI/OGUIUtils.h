//
// Created by damiles on 17/11/16.
//

#ifndef OPENCVGUI_OGUIUTILS_H
#define OPENCVGUI_OGUIUTILS_H

#include "OGUIWindow.h"

using namespace OpenCVGUI;

void drawParagraph(void* vvg, const char* text, float x, float y, float width, float height, float mx, float my);
bool drawBasicButton(void *context, OGUIWindow* window, const char* title, int x, int y, int width, int height, int mouse_x, int mouse_y );
void drawHeader(void* vvg, string title, int x, int y, int w);

#endif //OPENCVGUI_OGUIUTILS_H
