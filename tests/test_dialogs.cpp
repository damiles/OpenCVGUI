// Blocking dialogs: they must return right away in the UI thread and
// release the process thread when the window is closed.
#include "OGUI.h"
#include <GLFW/glfw3.h>
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

using namespace OpenCVGUI;

static int failures = 0;

static void check(bool condition, const std::string& message) {
    std::cout << (condition ? "[ OK ] " : "[FAIL] ") << message << std::endl;
    if (!condition)
        failures++;
}

int main() {
    if (!OpenCVGUI::init()) {
        std::cerr << "Can not initialize GLFW" << std::endl;
        return 1;
    }
    OGUICVWindow* window = OpenCVGUI::namedWindow("test dialogs", 640, 480);

    // Waiting here would block the UI forever
    check(window->popup("title", "text", POPUP_ALERT) == 0, "popup from the UI thread returns");
    check(window->openFileBrowser(".", {"*"}).empty(), "file browser from the UI thread returns");

    std::atomic<int> popup_result(-1);
    std::atomic<bool> browser_returned(false);
    std::string browser_result = "not set";

    std::thread closer([window]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        glfwSetWindowShouldClose(window->glfw_window, 1);
    });

    OpenCVGUI::app_run([&]() {
        popup_result = window->popup("title", "waiting popup", POPUP_CONFIRM);
        browser_result = window->openFileBrowser(".", {"*"});
        browser_returned = true;
    });
    closer.join();

    check(popup_result == 0, "popup returns 0 when the window is closed");
    check(browser_returned && browser_result.empty(), "file browser returns an empty path when the window is closed");
    return failures == 0 ? 0 : 1;
}
