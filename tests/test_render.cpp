// Render images, plots and widgets from the process thread and check the
// framebuffer: the red image must be on screen and the 3D surface drawn.
#include "OGUI.h"
#include "OGUI3D.h"
#include "OGUIButton.h"
#include "OGUICheckBox.h"
#include "OGUISlider.h"
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
    OGUICVWindow* window = OpenCVGUI::namedWindow("test render", 800, 600);

    OGUISlider slider("Slider", 0, 255, 125);
    OGUICheckBox checkbox("CheckBox", true);
    OGUIButton button("Button");
    window->addFormWidget(&slider);
    window->addFormWidget(&checkbox);
    window->addFormWidget(&button);

    std::atomic<int> ui_tasks(0);
    std::atomic<int> red_pixels(-1);
    std::atomic<int> blue_pixels(-1);
    std::atomic<int> surface_pixels(-1);
    std::atomic<bool> pixels_read(false);

    OpenCVGUI::app_run([&]() {
        // BGR red, a gray image, a BGRA blue image and every plot type
        cv::Mat red(240, 320, CV_8UC3, cv::Scalar(0, 0, 255));
        cv::Mat gray(240, 320, CV_8UC1);
        cv::randu(gray, 0, 255);
        cv::Mat bgra(120, 160, CV_8UC4, cv::Scalar(255, 0, 0, 255));

        cv::Mat series(2, 50, CV_32F);
        cv::randu(series, 0.f, 1.f);
        cv::Mat points(1, 50, CV_32FC2);
        cv::randu(points, 0.f, 1.f);
        cv::Mat surface(40, 40, CV_32F);
        for (int r = 0; r < surface.rows; r++)
            for (int c = 0; c < surface.cols; c++)
                surface.at<float>(r, c) = std::sin(r * 0.2f) * std::cos(c * 0.2f);

        for (int frame = 0; frame < 30 && app_is_running; frame++) {
            window->imshow("red", red);
            window->imshow("gray", gray);
            window->imshow("bgra", bgra);
            window->plot2D("series", series, {"a", "b"});
            window->plot2D("bars", series, {"a", "b"}, 1, PLOT_BARS);
            window->plot2D("line", points, {"xy"}, 1, PLOT_LINE);
            window->plot2D("scatter", points, {"xy"}, 1, PLOT_SCATTER);
            window->plot3D("surface", surface);
            window->show_progress("progress", frame / 30.f);
            window->runOnUIThread([&]() { ui_tasks++; });
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        window->hide_progress();

        // Read the framebuffer after the 2D pass and the 3D surface are drawn
        window->runOnUIThread([&]() {
            OGUI3D* area = dynamic_cast<OGUI3D*>(window->findAreaByTitle("surface"));
            if (area == NULL)
                return;
            area->setExternalDraw3D([&](int, int, int, int, const glm::mat4&) {
                if (pixels_read)
                    return;
                int w = window->getWindowWidth(), h = window->getWindowHeight();
                cv::Mat pixels(h, w, CV_8UC4);
                glPixelStorei(GL_PACK_ALIGNMENT, 1);
                glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data);
                cv::flip(pixels, pixels, 0);

                // Pixels of an RGB color inside the area, areas use window coordinates
                auto count_color = [&](const std::string& title, cv::Scalar rgb) {
                    OGUIArea* image_area = window->findAreaByTitle(title);
                    cv::Rect rect = cv::Rect(image_area->x, image_area->y, image_area->width, image_area->height)
                                    & cv::Rect(0, 0, w, h);
                    cv::Mat mask;
                    cv::inRange(pixels(rect), rgb - cv::Scalar(40, 40, 40, 255), rgb + cv::Scalar(40, 40, 40, 255), mask);
                    return cv::countNonZero(mask);
                };
                blue_pixels = count_color("bgra", cv::Scalar(0, 0, 255, 255));
                red_pixels = count_color("red", cv::Scalar(255, 0, 0, 255));

                // Only the 3D pass writes depth, NanoVG does not
                GLint viewport[4];
                glGetIntegerv(GL_VIEWPORT, viewport);
                cv::Mat depth(viewport[3], viewport[2], CV_32F);
                glReadPixels(viewport[0], viewport[1], viewport[2], viewport[3],
                             GL_DEPTH_COMPONENT, GL_FLOAT, depth.data);
                surface_pixels = cv::countNonZero(depth < 1.f);
                pixels_read = true;
            });
        });

        for (int i = 0; i < 500 && !pixels_read && app_is_running; i++)
            std::this_thread::sleep_for(std::chrono::milliseconds(10));

        glfwSetWindowShouldClose(window->glfw_window, 1);
    });

    std::cout << "red pixels " << red_pixels << ", blue pixels " << blue_pixels
              << ", surface pixels " << surface_pixels << std::endl;
    check(ui_tasks == 30, "runOnUIThread runs every task");
    check(window->areas_showing.size() == 8, "one area for each image and plot");
    check(red_pixels > 5000, "BGR image is drawn with its colors");
    check(blue_pixels > 2000, "BGRA image is drawn with its colors");
    check(surface_pixels > 1000, "3D surface is drawn");
    return failures == 0 ? 0 : 1;
}
