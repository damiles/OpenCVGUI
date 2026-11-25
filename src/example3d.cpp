#include <iostream>
#include <atomic>
#include <thread>
#include <chrono>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "OGUI.h"
#include "OGUICVWindow.h"
#include "OGUIButton.h"

using namespace OpenCVGUI;
using namespace cv;

OGUICVWindow *window;
std::atomic<bool> show_popup_help(false);

void hello_btn_click(){
    show_popup_help= true;
}

void cv_process() {
    while(app_is_running) {
        if(show_popup_help) {
            window->popup("3D Sample", "This sample shows a 3D plot from an OpenCV Mat and a custom 3D drawing that follows the camera", POPUP_ALERT);
            show_popup_help= false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// Custom 3D drawing: XYZ axes with its own shader, called in the UI thread
GLuint axes_program= 0, axes_vao= 0, axes_vbo= 0;
GLint axes_mvp_id;

void init_axes() {
    const char* vert_src= R"(#version 150
in vec3 position;
in vec3 color;
uniform mat4 MVP;
out vec3 v_color;
void main() {
    v_color= color;
    gl_Position= MVP*vec4(position, 1.0);
}
)";
    const char* frag_src= R"(#version 150
in vec3 v_color;
out vec4 out_color;
void main() {
    out_color= vec4(v_color, 1.0);
}
)";
    GLuint vert= glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vert_src, NULL);
    glCompileShader(vert);
    GLuint frag= glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &frag_src, NULL);
    glCompileShader(frag);

    axes_program= glCreateProgram();
    glAttachShader(axes_program, vert);
    glAttachShader(axes_program, frag);
    glBindAttribLocation(axes_program, 0, "position");
    glBindAttribLocation(axes_program, 1, "color");
    glLinkProgram(axes_program);
    glDeleteShader(vert);
    glDeleteShader(frag);
    axes_mvp_id= glGetUniformLocation(axes_program, "MVP");

    // position xyz, color rgb
    GLfloat lines[]= {
        0,0,0, 1,0,0,   1,0,0, 1,0,0,
        0,0,0, 0,1,0,   0,1,0, 0,1,0,
        0,0,0, 0,0,1,   0,0,1, 0,0,1,
    };
    glGenVertexArrays(1, &axes_vao);
    glBindVertexArray(axes_vao);
    glGenBuffers(1, &axes_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, axes_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lines), lines, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
}

void custom_3d(int x, int y, int width, int height, const glm::mat4& mvp)
{
    if(axes_program==0)
        init_axes();

    glUseProgram(axes_program);
    glUniformMatrix4fv(axes_mvp_id, 1, GL_FALSE, &mvp[0][0]);
    glBindVertexArray(axes_vao);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glDrawArrays(GL_LINES, 0, 6);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

int main( int argc, const char* argv[] )
{
    OpenCVGUI::init();

    window= OpenCVGUI::namedWindow("Wellcome to OpenCVGUI Example 3D");

    OGUIButton button1("3D Sample");
    button1.setCallBack(hello_btn_click);
    window->addFormWidget(&button1);

    Mat indata= imread("../resources/3d.png", IMREAD_GRAYSCALE);
    if(indata.empty()){
        cerr << "Can not open ../resources/3d.png, run the example from the bin folder" << endl;
        return 1;
    }
    blur(indata, indata, Size(3,3));
    window->imshow("3D input data", indata);

    // 3D plot of the image with custom axes on top
    Mat data;
    indata.convertTo(data, CV_32F, 1/255.0);
    resize(data, data, Size(100,100));
    window->plot3D("3D plot", data);
    OGUI3D* plot3d= (OGUI3D*)window->findAreaByTitle("3D plot");
    plot3d->setExternalDraw3D(custom_3d);

    // 3D area without data, only custom drawing
    Mat empty_data;
    window->plot3D("Custom 3D", empty_data);
    OGUI3D* custom3d= (OGUI3D*)window->findAreaByTitle("Custom 3D");
    custom3d->setExternalDraw3D(custom_3d);

    OpenCVGUI::app_run(cv_process);

    return 0;
}
