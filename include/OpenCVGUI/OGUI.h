/**
 * Created by damiles on 23/09/16.
 *
 */

#ifndef OPENCVGUI_OGUI_H
#define OPENCVGUI_OGUI_H

#include <string>
#include <vector>
#include <functional>

#include <opencv2/core/core.hpp>
#include "OGUICVWindow.h"

namespace OpenCVGUI {
    /**
     * Create new Computer Vision Template Window with lateral form and stacked areas
     * @param {const char*} title of window to create
     * @param {int} width of window, 1024 by default
     * @param {int} height of window, 768 by default
     * @return {OGUICVWindow*} OGUICVWindow created
     *
     */
    OGUICVWindow* namedWindow(const char* title, int width=1024, int height=768);

    /**
     * Start main loop of UI and processing stuff as new thread.
     * @param process_function function for processing computer vision thread.
     */
    void app_run(std::function<void()> process_function);

    /**
     * variable to check if the UI is running or closed and finished
     */
    extern bool app_is_running;

    /**
     * list of windows that are created
     */
    extern vector<OGUIWindow*> windows;

}
#endif //OPENCVGUI_OGUI_H
