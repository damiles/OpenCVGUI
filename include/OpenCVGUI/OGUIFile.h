//
// Created by damiles on 24/11/16.
//

#ifndef OPENCVGUI_OGUIFILE_H
#define OPENCVGUI_OGUIFILE_H

#include <string>

using namespace std;

namespace OpenCVGUI {
    class OGUIFile {
    public:
        OGUIFile(void* window, const char* file_name, const char* path, int isdir, const char* ext, double size);
        string _file_name;
        string _path;
        string _ext;
        bool  _is_dir;
        double _size;
        void* _window;

        bool draw(void *vvg, int x, int y, int mouse_x, int mouse_y);

    private:
        int _mouse_state;

    };
}

#endif //OPENCVGUI_OGUIFILE_H
