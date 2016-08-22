//
// Created by damiles on 10/08/16.
//

#ifndef OPENCVGUI_OGUIRADIOBUTTONS_H
#define OPENCVGUI_OGUIRADIOBUTTONS_H

#include <vector>
#include <string>
#include <functional>
#include "OGUIWidget.h"

namespace OpenCVGUI {
    class OGUIRadioButtons : public OGUIWidget {
    public:
        virtual void draw(int x, int y, int width);
        OGUIRadioButtons(const char *title, std::vector<std::string> options, int default_selected=0);
        void setCallBack(std::function<void(int)> func);

    private:
        const char *title_;
        std::vector <std::string> options_;
        int selected_;

        std::function<void(int)> on_change_callback;
    };
}

#endif //OPENCVGUI_OGUIRADIOBUTTONS_H
