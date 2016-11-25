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
        OGUIFile(const char* file_name, const char* path, int isdir, const char* ext, double size);
        string _file_name;
        string _path;
        string _ext;
        bool  _is_dir;
        double _size;

        string draw(int x, int y);
    };
}

#endif //OPENCVGUI_OGUIFILE_H
