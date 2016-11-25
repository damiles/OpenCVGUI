//
// Created by damiles on 24/11/16.
//

#include "OGUIFile.h"

namespace OpenCVGUI {

    OGUIFile::OGUIFile(const char* file_name, const char* path, int isdir, const char* ext, double size) {
        _file_name= file_name;
        _path= path;
        _ext= ext;
        _size= size;
        _is_dir= (bool)isdir;
    }
}