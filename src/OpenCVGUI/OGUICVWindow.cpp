//
// Created by damiles on 27/09/16.
//

#include "OGUICVWindow.h"

namespace OpenCVGUI {

    OGUICVWindow::OGUICVWindow(int width,int height,const char* title, int layoutOrientation): OGUIWindow(width, height, title, layoutOrientation)
    {
        this->formArea= new OGUIFormArea(this);
        this->layout1= new OGUILayout(this, 1);
        this->layout2= new OGUILayout(this, 1);

        this->addArea(formArea);
        this->addArea(layout1);
        this->addArea(layout2);

        std::vector<float> sizes;
        sizes.push_back(0.2);
        sizes.push_back(0.4);
        sizes.push_back(0.4);
        this->getMainLayout()->setLayoutSizes(sizes);

    }

    void OGUICVWindow::imshow(void *img) {

    }


}