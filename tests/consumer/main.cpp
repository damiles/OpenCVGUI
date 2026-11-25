#include "OGUI.h"

int main() {
    if (!OpenCVGUI::init())
        return 1;
    OpenCVGUI::OGUICVWindow* window = OpenCVGUI::namedWindow("consumer", 320, 240);
    window->imshow("image", cv::Mat(120, 160, CV_8UC3, cv::Scalar(0, 0, 255)));
    for (int i = 0; i < 3; i++)
        window->update();
    return 0;
}
