/**
 * Created by damiles on 23/09/16.
 *
 */

#ifndef OPENCVGUI_OGUI_H
#define OPENCVGUI_OGUI_H

#include <string>
#include <vector>
#include <functional>

#include "common.h"

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
	CV_OGUI_EXPORTS OGUICVWindow* namedWindow(const char* title, int width=1024, int height=768);

    /**
     * Start main loop of UI and processing stuff as new thread.
     * @param process_function function for processing computer vision thread.
     */
	CV_OGUI_EXPORTS void app_run(std::function<void()> process_function);

    /**
     * variable to check if the UI is running or closed and finished
     */
	CV_OGUI_EXPORTS bool app_is_running();
	extern bool _app_is_running;

    /**
     * list of windows that are created
     */
	CV_OGUI_EXPORTS void add_window(OGUIWindow* window);
	extern vector<OGUIWindow*> windows;

}
#endif //OPENCVGUI_OGUI_H
