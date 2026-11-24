//
// Created by damiles on 17/11/16.
//

#ifndef OPENCVGUI_OGUIUTILS_H
#define OPENCVGUI_OGUIUTILS_H

#include "OGUIWindow.h"

using namespace OpenCVGUI;

void drawParagraph(NVGcontext* vg, const char* text, float x, float y, float width, float height, float mx, float my);
bool drawBasicButton(NVGcontext* vg, OGUIWindow* window, const char* title, int x, int y, int width, int height, int mouse_x, int mouse_y, const char* typography="sans", double font_size=20.0f );
void drawHeader(NVGcontext* vg, string title, int x, int y, int w);
void drawLabel(NVGcontext* vg, const char* text, int x, int y, const char* typography="sans", double font_size=16.0f, int r=255, int g=255, int b=255, int a=255 );
#endif //OPENCVGUI_OGUIUTILS_H
