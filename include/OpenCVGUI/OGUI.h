//
// Created by damiles on 23/09/16.
//

#ifndef OPENCVGUI_OGUI_H
#define OPENCVGUI_OGUI_H

#include <string>
#include <vector>
#include <functional>

#include <opencv2/core/core.hpp>
#include "OGUIWindow.h"

namespace OpenCVGUI {
    OGUIWindow* namedWindow(const char* title, int width=1024, int height=768);
    void imshow(const string title, cv::Mat img);
    void app_run(std::function<void()> process_function);
    bool app_is_running= true;

}
#endif //OPENCVGUI_OGUI_H
