#include <GL/glew.h>
#include "OGUI3D.h"
#include <iostream>
#include <fstream>
#include <nanovg.h>
#include "OGUIWindow.h"

#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace OpenCVGUI {

    // Read a shader source from a file
// store the shader source in a std::vector<char>
    void read_shader_src(const char *fname, std::vector<char> &buffer) {
        std::ifstream in;
        in.open(fname, std::ios::binary);

        if(in.is_open()) {
            // Get the number of bytes stored in this file
            in.seekg(0, std::ios::end);
            size_t length = (size_t)in.tellg();

            // Go to start of the file
            in.seekg(0, std::ios::beg);

            // Read the content of the file in a buffer
            buffer.resize(length + 1);
            in.read(&buffer[0], length);
            in.close();
            // Add a valid C - string end
            buffer[length] = '\0';
        }
        else {
            std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
            exit(-1);
        }
    }

// Compile a shader
    GLuint load_and_compile_shader(const char *fname, GLenum shaderType) {
        // Load a shader from an external file
        std::vector<char> buffer;
        read_shader_src(fname, buffer);
        const char *src = &buffer[0];

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
            exit(-1);
        }
        return shader;
    }

// Create a program from two shaders
    GLuint create_program(const char *path_vert_shader, const char *path_frag_shader) {
        // Load and compile the vertex and fragment shaders
        GLuint vertexShader = load_and_compile_shader(path_vert_shader, GL_VERTEX_SHADER);
        GLuint fragmentShader = load_and_compile_shader(path_frag_shader, GL_FRAGMENT_SHADER);

        // Attach the above shader to a program
        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);

        // Flag the shaders for deletion
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // Link and use the program
        glLinkProgram(shaderProgram);
        glUseProgram(shaderProgram);

        return shaderProgram;
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

   shaderProgram = create_program("shaders/vert.shader", "shaders/frag.shader");
//
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
    if(!isBufferCreated && data.data!=NULL)
    {
        CreateVertexBuffer();
        isBufferCreated= true;
    }

    glEnable(GL_DEPTH_TEST);
    glPointSize(2.f);
    // ToDo Enable GL3 version...
    glViewport(x, window->getWindowHeight()-height-y, (GLsizei)(width), (GLsizei)(height));

    glUseProgram(shaderProgram);

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

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    if(isBufferCreated) {
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
    if(externDraw3d!=NULL)
        externDraw3d(x, y, width, height);

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

    isBufferCreated= false;
}

void OGUI3D::replot(void *d)
{
    ((Mat*)d)->copyTo(data);
    isBufferCreated= false;
}

} /* End of namespace OpenCVGUI */
