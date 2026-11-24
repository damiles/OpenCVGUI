#include <GL/glew.h>
#include "OGUI3D.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <cstdlib>
#include <nanovg.h>
#include "OGUIWindow.h"

#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace OpenCVGUI {

    // Default shaders embedded in the library (same as resources/shaders)
    static const char* default_vert_shader = R"(#version 150

in vec4 position;
uniform mat4 MVP;
out float ypos;

void main() {
    ypos= position[1];
    gl_Position = MVP*position;
}
)";

    static const char* default_frag_shader = R"(#version 150

out vec4 out_color;
in float ypos;

void main() {
    gl_FragColor = vec4(cos(ypos), sin(ypos), ypos*ypos, 1.0);
}
)";

    // Directory with user custom shaders, empty to use OPENCVGUI_SHADERS_DIR or defaults
    static std::string shaders_dir;

    void setShadersDir(const std::string& dir) {
        shaders_dir= dir;
    }

    // Read a shader source from a file, return false if the file can not be read
    bool read_shader_src(const std::string &fname, std::string &buffer) {
        std::ifstream in(fname, std::ios::binary);
        if(!in.is_open())
            return false;
        buffer.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
        return true;
    }

    // Get the user custom shader source if exists, if not the default one
    std::string get_shader_src(const char *fname, const char *default_src, bool &is_custom) {
        std::string dir= shaders_dir;
        if(dir.empty()) {
            const char *env= getenv("OPENCVGUI_SHADERS_DIR");
            if(env!=NULL)
                dir= env;
        }

        std::string src;
        is_custom= !dir.empty() && read_shader_src(dir + "/" + fname, src);
        if(!is_custom)
            src= default_src;
        return src;
    }

// Compile a shader, return 0 if fails
    GLuint load_and_compile_shader(const std::string &source, GLenum shaderType) {
        const char *src = source.c_str();

        // Compile the shader
        GLuint shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);
        // Check the result of the compilation
        GLint test;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
        if(!test) {
            std::cerr << "Shader compilation failed with this message:" << std::endl;
            std::vector<char> compilation_log(512);
            glGetShaderInfoLog(shader, compilation_log.size(), NULL, &compilation_log[0]);
            std::cerr << &compilation_log[0] << std::endl;
            glDeleteShader(shader);
            return 0;
        }
        return shader;
    }

// Create a program from two shader sources, return 0 if fails
    GLuint create_program(const std::string &vert_src, const std::string &frag_src) {
        // Compile the vertex and fragment shaders
        GLuint vertexShader = load_and_compile_shader(vert_src, GL_VERTEX_SHADER);
        GLuint fragmentShader = load_and_compile_shader(frag_src, GL_FRAGMENT_SHADER);
        if(vertexShader==0 || fragmentShader==0) {
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return 0;
        }

        // Attach the above shader to a program
        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);

        // Flag the shaders for deletion
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Link and check the program
        glLinkProgram(shaderProgram);
        GLint test;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &test);
        if(!test) {
            std::cerr << "Shader program link failed with this message:" << std::endl;
            std::vector<char> link_log(512);
            glGetProgramInfoLog(shaderProgram, link_log.size(), NULL, &link_log[0]);
            std::cerr << &link_log[0] << std::endl;
            glDeleteProgram(shaderProgram);
            return 0;
        }
        glUseProgram(shaderProgram);

        return shaderProgram;
    }

// Create the 3D plot program with user custom shaders, falling back to defaults
    GLuint create_3d_program() {
        bool custom_vert, custom_frag;
        std::string vert_src= get_shader_src("vert.shader", default_vert_shader, custom_vert);
        std::string frag_src= get_shader_src("frag.shader", default_frag_shader, custom_frag);

        GLuint program= create_program(vert_src, frag_src);
        if(program==0 && (custom_vert || custom_frag)) {
            std::cerr << "Custom shaders failed, using default shaders" << std::endl;
            program= create_program(default_vert_shader, default_frag_shader);
        }
        return program;
    }



    OGUI3DCamera::OGUI3DCamera() {
        radius=3;
        phi= theta= 0.785398;
        // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        Projection = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);
        // Camera matrix
        x= radius*cos(phi)*sin(theta);
        y= radius*sin(phi)*sin(theta);
        z= radius*cos(theta);

        View       = glm::lookAt(
                glm::vec3(x,y,z), // Camera is at (4,3,-3), in World Space
                glm::vec3(0,0,0), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        // Model matrix : an identity matrix (model will be at the origin)
        Model      = glm::mat4(1.0f);
        MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around
    }
    glm::mat4 OGUI3DCamera::setRadius(float r) {
        radius=r;
        x= radius*cos(phi)*sin(theta);
        y= radius*sin(phi)*sin(theta);
        z= radius*cos(theta);

        View       = glm::lookAt(
                glm::vec3(x,y,z), // Camera is at (4,3,-3), in World Space
                glm::vec3(0,0,0), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        MVP= Projection * View * Model;
        return MVP;
    }
    glm::mat4 OGUI3DCamera::mouseEvent(int dx, int dy) {
        phi+= dy*(CV_PI/180.0f);
        theta+= dx*(CV_PI/180.0f);
        x= radius*cos(phi)*sin(theta);
        y= radius*sin(phi)*sin(theta);
        z= radius*cos(theta);

        View       = glm::lookAt(
                glm::vec3(x,y,z), // Camera is at (4,3,-3), in World Space
                glm::vec3(0,0,0), // and looks at the origin
                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        MVP= Projection * View * Model;
        return MVP;
    }
    glm::mat4 OGUI3DCamera::getMVP() {
        return MVP; // Remember, matrix multiplication is the other way around
    }




    void OGUI3D::CreateVertexBuffer()
{
    // Release previous buffers if it is a replot
    if(vao!=0) {
        glDeleteVertexArrays(1, &vao);
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }

    // Use a Vertex Array Object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create points from Mat
    dataLength= data.cols*data.rows*3;
    GLfloat vertices_position[dataLength];
    int index=0;
    for(int x=0; x<data.cols; x++){
        for(int y=0; y<data.rows; y++){

            vertices_position[index]= x/(float)data.cols;
            vertices_position[index+1]= data.at<float>(y,x);
            vertices_position[index+2]= y/(float)data.cols;

            index+=3;
        }
    }


    // Create a Vector Buffer Object that will store the vertices on video memory
    glGenBuffers(1, &vbo);

    // Allocate space and upload the data from CPU to GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position), vertices_position, GL_STATIC_DRAW);


    // INDEX BUFFERS
    dataLengthIBO= (data.cols-1)*(data.rows-1)*3*2;
    unsigned int indexs[dataLengthIBO];
    index=0;
    for(int x=0; x<data.cols-1; x++){
        for(int y=0; y<data.rows-1; y++){
            int i=y*data.cols + x;
            indexs[index]= i;
            indexs[index+1]= i+1;
            indexs[index+2]= i+data.cols;

            indexs[index+3]= i+1;
            indexs[index+4]= i+data.cols;
            indexs[index+5]= i+data.cols+1;

            index+=6;
        }
    }

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexs), indexs, GL_STATIC_DRAW);


    // SHADER PROGRAM

    if(shaderProgram!=0)
        glDeleteProgram(shaderProgram);
    shaderProgram = create_3d_program();
    if(shaderProgram==0)
        return;

    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(shaderProgram, "MVP");

    MVP= camera.getMVP();


    // Get the location of the attributes that enters in the vertex shader
    position_attribute = glGetAttribLocation(shaderProgram, "position");



        // Specify how the data for position can be accessed
    glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);


}

    void OGUI3D::updateScrollStatus(double xoffset,double yoffset)
    {
        if(isMouseIn()) {
            camera.setRadius(camera.radius + (yoffset / 2.));
        }
    }

void OGUI3D::draw3d(int x, int y, int width, int height)
{
    bool has_plot;
    std::function<void(int, int, int, int, const glm::mat4&)> external_draw;
    {
        std::lock_guard<std::mutex> lock(_mutex);
        // Areas without data are only used for external draw
        if(!isBufferCreated && !data.empty())
        {
            CreateVertexBuffer();
            isBufferCreated= true;
        }
        has_plot= isBufferCreated && !data.empty();
        external_draw= _externalDraw3D;
    }

    glEnable(GL_DEPTH_TEST);
    glPointSize(2.f);
    // ToDo Enable GL3 version...
    glViewport(x, window->getWindowHeight()-height-y, (GLsizei)(width), (GLsizei)(height));

    if(isMouseIn()) {
        if (window->mouse_state == GLFW_PRESS){
            int dy=0;
            int dx=0;
            if(lastMouseY!=-1) {
                dy = window->mouse_y - lastMouseY;
                dx = window->mouse_x - lastMouseX;
                camera.mouseEvent(dx,dy);
            }

            lastMouseX= window->mouse_x;
            lastMouseY= window->mouse_y;

        }else{
            lastMouseX= lastMouseY=-1;
        }
    }

    MVP= camera.getMVP();

    // No valid shader program or data, only external draw
    if(has_plot && shaderProgram!=0)
        drawPlot();

    if(external_draw!=NULL)
        external_draw(x, y, width, height, MVP);
}

void OGUI3D::drawPlot()
{
    glUseProgram(shaderProgram);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // Get the location of the attributes that enters in the vertex shader
    position_attribute = glGetAttribLocation(shaderProgram, "position");

    // Enable the attribute
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Specify how the data for position can be accessed
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glDrawElements(GL_TRIANGLES, dataLengthIBO, GL_UNSIGNED_INT, 0);
    //glDrawArrays(GL_POINTS, 0, dataLength/3);

    glDisableVertexAttribArray(0);

}

void OGUI3D::draw(int x, int y, int width, int height)
{
    this->x=x;
    this->y=y;
    this->width= width;
    this->height= height;
    NVGcontext* vg= (NVGcontext*)(window->vg);

    nvgScissor(vg, x, y, width, height);
    OGUIArea::draw(x,y,width,height);

    drawTitle();
    nvgResetScissor(vg);
}

OGUI3D::OGUI3D(OGUIWindow* window, std::string title, void* d): OGUIArea(window)
{
    this->type= 1; // 3d type window
    this->title= title;
    ((Mat*)d)->copyTo(data);

    shaderProgram= 0;
    vao= vbo= ibo= 0;
    isBufferCreated= false;
}

void OGUI3D::setExternalDraw3D(std::function<void(int x, int y, int width, int height, const glm::mat4& mvp)> func)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _externalDraw3D= func;
}

void OGUI3D::replot(void *d)
{
    // Copy outside the lock, the lock only swaps the data
    Mat new_data;
    ((Mat*)d)->copyTo(new_data);
    std::lock_guard<std::mutex> lock(_mutex);
    data= new_data;
    isBufferCreated= false;
}

} /* End of namespace OpenCVGUI */
