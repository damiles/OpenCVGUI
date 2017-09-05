//
// Created by damiles on 17/11/16.
//
#include "OGUIUtils.h"

#include <stdio.h>
#include <string>
#include <cstring>
#include <math.h>
#include <vector>
#include <iostream>
//#include <unistd.h>
#include <iostream>

#include "nanovg.h"

using namespace std;

//static float minf(float a, float b) { return a < b ? a : b; }
static float maxf(float a, float b) { return a > b ? a : b; }
//static float absf(float a) { return a >= 0.0f ? a : -a; }
static float clampf(float a, float mn, float mx) { return a < mn ? mn : (a > mx ? mx : a); }

void drawParagraph(void* vvg, const char* text, float x, float y, float width, float height, float mx, float my)
{
    NVGcontext* vg= (NVGcontext*)vvg;
    NVGtextRow rows[3];
    NVGglyphPosition glyphs[100];
    const char* start;
    const char* end;
    int nrows, i, nglyphs, j, lnum = 0;
    float lineh;
    float caretx, px;
    float bounds[4];
    float a;
    float gx,gy;
    int gutter = 0;
    NVG_NOTUSED(height);

    nvgSave(vg);

    nvgFontSize(vg, 18.0f);
    nvgFontFace(vg, "sans");
    nvgTextAlign(vg, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
    nvgTextMetrics(vg, NULL, NULL, &lineh);

    // The text break API can be used to fill a large buffer of rows,
    // or to iterate over the text just few lines (or just one) at a time.
    // The "next" variable of the last returned item tells where to continue.
    start = text;
    end = text + strlen(text);
    while ((nrows = nvgTextBreakLines(vg, start, end, width, rows, 3))) {
        for (i = 0; i < nrows; i++) {
            NVGtextRow* row = &rows[i];
            int hit = mx > x && mx < (x+width) && my >= y && my < (y+lineh);

            nvgBeginPath(vg);
            nvgFillColor(vg, nvgRGBA(255,255,255,hit?64:16));
            nvgRect(vg, x, y, row->width, lineh);
            nvgFill(vg);

            nvgFillColor(vg, nvgRGBA(255,255,255,255));
            nvgText(vg, x, y, row->start, row->end);

            /* // This is for mouse detection
             * if (hit) {
                caretx = (mx < x+row->width/2) ? x : x+row->width;
                px = x;
                nglyphs = nvgTextGlyphPositions(vg, x, y, row->start, row->end, glyphs, 100);
                for (j = 0; j < nglyphs; j++) {
                    float x0 = glyphs[j].x;
                    float x1 = (j+1 < nglyphs) ? glyphs[j+1].x : x+row->width;
                    float gx = x0 * 0.3f + x1 * 0.7f;
                    if (mx >= px && mx < gx)
                        caretx = glyphs[j].x;
                    px = gx;
                }
                nvgBeginPath(vg);
                nvgFillColor(vg, nvgRGBA(255,192,0,255));
                nvgRect(vg, caretx, y, 1, lineh);
                nvgFill(vg);

                gutter = lnum+1;
                gx = x - 10;
                gy = y + lineh/2;
            }*/
            lnum++;
            y += lineh;
        }
        // Keep going...
        start = rows[nrows-1].next;
    }

    if (gutter) {
        char txt[16];
        snprintf(txt, sizeof(txt), "%d", gutter);
        nvgFontSize(vg, 13.0f);
        nvgTextAlign(vg, NVG_ALIGN_RIGHT|NVG_ALIGN_MIDDLE);

        nvgTextBounds(vg, gx,gy, txt, NULL, bounds);

        nvgBeginPath(vg);
        nvgFillColor(vg, nvgRGBA(255,192,0,255));
        nvgRoundedRect(vg, (int)bounds[0]-4,(int)bounds[1]-2, (int)(bounds[2]-bounds[0])+8, (int)(bounds[3]-bounds[1])+4, ((int)(bounds[3]-bounds[1])+4)/2-1);
        nvgFill(vg);

        nvgFillColor(vg, nvgRGBA(32,32,32,255));
        nvgText(vg, gx,gy, txt, NULL);
    }

    y += 20.0f;

    /*nvgFontSize(vg, 13.0f);
    nvgTextAlign(vg, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
    nvgTextLineHeight(vg, 1.2f);

    nvgTextBoxBounds(vg, x,y, 150, "Hover your mouse over the text to see calculated caret position.", NULL, bounds);

    // Fade the tooltip out when close to it.
    gx = fabsf((mx - (bounds[0]+bounds[2])*0.5f) / (bounds[0] - bounds[2]));
    gy = fabsf((my - (bounds[1]+bounds[3])*0.5f) / (bounds[1] - bounds[3]));
    a = maxf(gx, gy) - 0.5f;
    a = clampf(a, 0, 1);
    nvgGlobalAlpha(vg, a);

    nvgBeginPath(vg);
    nvgFillColor(vg, nvgRGBA(220,220,220,255));
    nvgRoundedRect(vg, bounds[0]-2,bounds[1]-2, (int)(bounds[2]-bounds[0])+4, (int)(bounds[3]-bounds[1])+4, 3);
    px = (int)((bounds[2]+bounds[0])/2);
    nvgMoveTo(vg, px,bounds[1] - 10);
    nvgLineTo(vg, px+7,bounds[1]+1);
    nvgLineTo(vg, px-7,bounds[1]+1);
    nvgFill(vg);

    nvgFillColor(vg, nvgRGBA(0,0,0,220));
    nvgTextBox(vg, x,y, 150, "Hover your mouse over the text to see calculated caret position.", NULL);
*/
    nvgRestore(vg);
}




bool drawBasicButton(void *context, OGUIWindow* window, const char* title, int x, int y, int width, int height, int mouse_x, int mouse_y, const char* typography, double font_size )
{
    NVGcontext* tmp_vg=(NVGcontext*) context;
    bool internal_state= false;
    nvgBeginPath(tmp_vg);
    nvgRoundedRect(tmp_vg, x,y, width,height, 3);
    if(mouse_x > x &&
       mouse_x < x+width &&
       mouse_y > y &&
       mouse_y < y + height
            ) {

        nvgFillColor(tmp_vg, nvgRGBA(50,53,58,255));
        window->setCursor(HAND_CURSOR);
        if(window->mouse_state == 1){
			cout << "Button preded " << title << endl; 
            internal_state=true;
        }

    }else {
        nvgFillColor(tmp_vg, nvgRGBA(50,53,58,180));
        window->setCursor(DEFAULT_CURSOR);
    }
    nvgFill(tmp_vg);

    nvgBeginPath(tmp_vg);
    nvgRoundedRect(tmp_vg, x+0.5f,y+0.5f, width, height, 3.5);
    nvgStrokeColor(tmp_vg, nvgRGBA(0,0,0,48));
    nvgStroke(tmp_vg);

    // Draw text
    nvgFontSize(tmp_vg, font_size);
    nvgFontFace(tmp_vg, typography);
    float tw = nvgTextBounds(tmp_vg, 0,0, title, NULL, NULL);
    nvgTextAlign(tmp_vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
    nvgFillColor(tmp_vg, nvgRGBA(0,0,0,255));
    nvgText(tmp_vg, x+width*0.5f-tw*0.5f,y+height*0.5f-1, title, NULL);
    nvgFillColor(tmp_vg, nvgRGBA(255,255,255,180));
    nvgText(tmp_vg, x+width*0.5f-tw*0.5f,y+height*0.5f, title, NULL);

    return internal_state;
}

void drawHeader(void* vvg, string title, int x, int y, int w){
    NVGpaint headerPaint;
    float cornerRadius = 3.0f;
    NVGcontext* vg=(NVGcontext*) vvg;
    // Header
    headerPaint = nvgLinearGradient(vg, x,y,x,y+15, nvgRGBA(255,255,255,8), nvgRGBA(0,0,0,16));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+1,y+1, w-2,30, cornerRadius-1);
    nvgFillPaint(vg, headerPaint);
    nvgFill(vg);
    nvgBeginPath(vg);
    nvgMoveTo(vg, x+0.5f, y+0.5f+30);
    nvgLineTo(vg, x+0.5f+w-1, y+0.5f+30);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,32));
    nvgStroke(vg);

    nvgFontSize(vg, 18.0f);
    nvgFontFace(vg, "sans-bold");
    nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

    nvgFontBlur(vg,2);
    nvgFillColor(vg, nvgRGBA(0,0,0,128));
    nvgText(vg, x+w/2,y+16+1, title.c_str(), NULL);

    nvgFontBlur(vg,0);
    nvgFillColor(vg, nvgRGBA(220,220,220,190));
    nvgText(vg, x+w/2,y+16, title.c_str(), NULL);

}

void drawLabel(void *context, const char* text, int x, int y, const char* typography, double font_size, int r, int g, int b, int a )
{
    NVGcontext* tmp_vg=(NVGcontext*) context;
    // Draw text
    nvgFontSize(tmp_vg, font_size);
    nvgFontFace(tmp_vg, typography);
    nvgTextAlign(tmp_vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
    nvgFillColor(tmp_vg, nvgRGBA(r,g,b,a));
    nvgText(tmp_vg, x,y, text, NULL);

}