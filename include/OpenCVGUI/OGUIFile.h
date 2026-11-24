//
// Created by damiles on 24/11/16.
//

#ifndef OPENCVGUI_OGUIFILE_H
#define OPENCVGUI_OGUIFILE_H

#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

using namespace std;
using namespace cv;

struct NVGcontext;

namespace OpenCVGUI {
    class OGUIWindow;

    class OGUIFile {
    public:
        OGUIFile(OGUIWindow* window, const char* file_name, const char* path, int isdir, const char* ext, double size);
        string _file_name;
        string _path;
        string _ext;
        int  _is_dir; //1 is dir 2 is drive
        double _size;
        OGUIWindow* _window;

        bool draw(NVGcontext* vg, int x, int y, int mouse_x, int mouse_y);

        bool isImage();
        unsigned char* getPreview();

    private:
        int _mouse_state;
        unsigned char* _preview_data;
    };
}

#endif //OPENCVGUI_OGUIFILE_H
