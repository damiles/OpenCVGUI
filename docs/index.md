---
# You don't need to edit this file, it's empty on purpose.
# Edit theme's home layout instead if you wanna make some changes
# See: https://jekyllrb.com/docs/themes/#overriding-theme-defaults
layout: hero
title: OpenCVGUI a new graphical user interface for OpenCV. Easy, powerful and profesional.
id: home
---
# OpenCVGUI
Do you like OpenCV Highui but it's not enough and don't want waste your time
in a new UI SDK? **This is  your solution!**
OpenCVGUI is a new graphical user inteface for OpenCV with different 
widgets and tools to provide developers a new and more powerful 
user interface with 3D and 2D plotting support, advanced forms, 
with small dependencies and OpenGL based.

Why OpenCVGUI: more Powerful and exactly so simple than OpenCV Highui.

![screenshot](https://github.com/damiles/OpenCVGUI/raw/master/resources/screenshot.png)

<iframe width="560" height="315" src="https://www.youtube.com/embed/QHKTZtBCSWs?ecver=1" frameborder="0" allowfullscreen></iframe>

<iframe width="560" height="315" src="https://www.youtube.com/embed/K-8mz9q6_zU?ecver=1" frameborder="0" allowfullscreen></iframe>

# Dependencies
* GLEW
* xorg-dev 

# Build
```bash
git submodule update --init --recursive
mkdir build
cd build
cmake ..
make
sudo make install
```

## Features

* No complex sdk framework like others. 
* It's like OpenCV Highui calls, simple and clean.
* Computer vision in different Thread than UI
* Image render
* Forms
* Input Text
* Buttons
* Radio buttons
* Checkboxs
* Sliders and progress bars
* File Management (in progress)
* 2D plot
* 2D multiplot
* Scatter plot
* Bar Plot
* 3D Plot

![screenshot](https://github.com/damiles/OpenCVGUI/raw/master/resources/2dplot.png)
![screenshot](https://github.com/damiles/OpenCVGUI/raw/master/resources/2dmultiplot.png)

## Short snippets

Init the GUI
```C++
    OpenCVGUI::init();
```

Create a new window
```C++
    OGUICVWindow* window= OpenCVGUI::namedWindow("Wellcome to OpenCVGUI Example 2");
```

Show image in window
```C++
    Mat image= imread("./lena.jpg");
    window->imshow("image title key like highui imshow", &image);
```

## Basic Example

```c++
#include "OGUI.h"
#include "OGUISlider.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace OpenCVGUI;
using namespace cv;

VideoCapture cap;
Mat frame, gray;
double thr=125;
OGUICVWindow *window;

void cv_process() {
    while(app_is_running) {
        cap >> frame;
        window->imshow("camera", &frame);
        Mat output;
        cvtColor( frame, gray, COLOR_RGB2GRAY );
        threshold( gray, output, (int)thr, 255, CV_THRESH_BINARY);
        window->imshow("threshold", &output);
    }
}

void on_change_slider_threshold(double value){
    thr= value;
}

int main( int argc, const char* argv[] )
{
    // Init GUI
    OpenCVGUI::init();

    // Create our window
    window= OpenCVGUI::namedWindow("Wellcome to OpenCVGUI Example 2");

    // Create the Forms we need and add to window
    OGUISlider slider("Threshold", 0, 255, 125);
    slider.setCallBack(on_change_slider_threshold);
    window->addFormWidget(&slider);

    // Start video capture
    cap.open(0);
    if(cap.isOpened()){
        cap >> frame;
        // show image
        window->imshow("camera", &frame);
    }else{
        return 0;
    }

    // Convert frame, threshodl and show
    cvtColor( frame, gray, COLOR_RGB2GRAY );
    Mat output;
    threshold( gray, output, 125, 255, CV_THRESH_BINARY);
    window->imshow("threshold", &output);

    // Start the Comptuer vision process
    OpenCVGUI::app_run(cv_process);

}    
```

![screenshot](https://github.com/damiles/OpenCVGUI/raw/master/resources/basic_sample.png)
