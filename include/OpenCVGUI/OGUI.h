//
// Created by damiles on 23/09/16.
//

#ifndef OPENCVGUI_OGUI_H
#define OPENCVGUI_OGUI_H

#include <string>
#include <vector>
#include <functional>

#include <opencv2/core/core.hpp>
#include "OGUICVWindow.h"

namespace OpenCVGUI {
    OGUICVWindow* namedWindow(const char* title, int width=1024, int height=768);
    void app_run(std::function<void()> process_function);
    extern bool app_is_running;
    extern vector<OGUIWindow*> windows;

}
#endif //OPENCVGUI_OGUI_H
