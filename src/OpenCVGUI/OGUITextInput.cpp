#include "OGUITextInput.h"
#include "OGUIArea.h"
#include "OGUIWindow.h"

#include <GL/glew.h>
#ifdef __APPLE__
#   define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>
#include "nanovg.h"

namespace OpenCVGUI {




void OGUITextInput::draw(int x, int y, int w)
{
    int h = this->height;
    OGUIWidget::draw(x, y, w);
    OGUIArea* area= (OGUIArea*)(this->area);
    NVGcontext* vg= (NVGcontext*)(area->window->vg);

    NVGpaint bg;
    // Edit
    bg = nvgBoxGradient(vg, x+1,y+1+1.5f, w-2,h-2, 3,4, nvgRGBA(255,255,255,32), nvgRGBA(32,32,32,32));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+1,y+1, w-2,h-2, 4-1);
    nvgFillPaint(vg, bg);
    nvgFill(vg);

    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+0.5f,y+0.5f, w-1,h-1, 4-0.5f);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,48));
    nvgStroke(vg);

    nvgFontSize(vg, 20.0f*area->window->font_scale);
    nvgFontFace(vg, "sans");
    if(value.compare(label)==0)
        nvgFillColor(vg, nvgRGBA(255,255,255,64));
    else
        nvgFillColor(vg, nvgRGBA(255,255,255,128));
    nvgTextAlign(vg,NVG_ALIGN_LEFT|NVG_ALIGN_MIDDLE);
    nvgText(vg, x+h*0.3f,y+h*0.5f, value.c_str(), NULL);

    if(area->window->mouse_x > x &&
       area->window->mouse_x < x+w &&
       area->window->mouse_y > y &&
       area->window->mouse_y < y + h
            ) {
        area->window->setCursor(TEXT_CURSOR);
        if (area->window->mouse_state == GLFW_PRESS)
        {
            is_focus_= true;
            area->window->setKeyFocus(this);
            if(value.compare(label)==0)
                value="";
        }
    }else{
        area->window->setCursor(DEFAULT_CURSOR);
        if (area->window->mouse_state == GLFW_PRESS)
        {
            if(value.compare("")==0)
                value= string(label);
            is_focus_= false;
        }
    }

    if(is_focus_){
        float char_pos= nvgTextBounds(vg, x, y, value.c_str(), NULL, NULL);
        nvgBeginPath(vg);
        animation_alpha_+=10;
        animation_alpha_=animation_alpha_%255;
        nvgFillColor(vg, nvgRGBA(255,192,0,animation_alpha_));
        nvgRect(vg, x+h*0.3f+char_pos, y+6, 1, 20);
        nvgFill(vg);
    }

}

OGUITextInput::OGUITextInput( const char* label)
{
    this->label= label;
    this->value= string(label);
    this->height= 30;
    this->is_focus_= false;
    animation_alpha_=0;
}

void OGUITextInput::characterCallback(unsigned int key)
{
    char c= static_cast<char>(key);
    value+=c;
}

void OGUITextInput::keyCallback(int key, int scancode, int action, int mods) {
    // ToDo David Millan: add suport for move and change letters in any place of string
    switch(key){
        case GLFW_KEY_BACKSPACE:
            if(value.size()>0)
                value.pop_back();
            break;
    }

}

} /* End of namespace OpenCVGUI */
