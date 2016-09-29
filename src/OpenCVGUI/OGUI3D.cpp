#include <GL/glew.h>
#include "OGUI3D.h"
#include <iostream>
#include <fstream>
#include <nanovg.h>
#include "OGUIWindow.h"

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



    void OGUI3D::CreateVertexBuffer()
{
    // Use a Vertex Array Object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // 4 triangles to be rendered
    GLfloat vertices_position[12] = {
            -0.5, -0.5, 0,
            0.5, -0.5, 0,
            0.5, 0.5, 0,
            -0.5, 0.5, 0
    };

    // Create a Vector Buffer Object that will store the vertices on video memory
    glGenBuffers(1, &vbo);

    // Allocate space and upload the data from CPU to GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position), vertices_position, GL_STATIC_DRAW);

   shaderProgram = create_program("shaders/vert.shader", "shaders/frag.shader");
//
    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(shaderProgram, "MVP");

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View       = glm::lookAt(
            glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    // Model matrix : an identity matrix (model will be at the origin)
    glm::mat4 Model      = glm::mat4(1.0f);
    // Our ModelViewProjection : multiplication of our 3 matrices
    MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around


    // Get the location of the attributes that enters in the vertex shader
    position_attribute = glGetAttribLocation(shaderProgram, "position");

    // Specify how the data for position can be accessed
    glVertexAttribPointer(position_attribute, 3, GL_FLOAT, GL_FALSE, 0, 0);


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

    if(isMouseIn()) {
        nvgBeginPath(vg);
        nvgRect(vg, x, y, width, 22);
        nvgFillColor(vg, nvgRGBA(0, 0, 0, 100));
        nvgFill(vg);

        // Draw text
        nvgFontSize(vg, 16.0f);
        nvgFontFace(vg, "sans");
        float tw = nvgTextBounds(vg, 0, 0, title.c_str(), NULL, NULL);
        nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
        nvgFillColor(vg, nvgRGBA(255, 255, 255, 255));
        nvgText(vg, x + width * 0.5f - tw * 0.5f, y + 11, title.c_str(), NULL);
    }
    nvgResetScissor(vg);
}

void OGUI3D::draw3d(int x, int y, int width, int height)
{

    glPointSize(8.f);
    // ToDo Enable GL3 version...
    // glViewport(x, window->getWindowHeight()-height-y, (GLsizei)(width), (GLsizei)(height));

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    // Get the location of the attributes that enters in the vertex shader
    position_attribute = glGetAttribLocation(shaderProgram, "position");

    // Enable the attribute
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Specify how the data for position can be accessed
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_POINTS, 0, 4);
    glDisableVertexAttribArray(0);

}

OGUI3D::OGUI3D(OGUIWindow* window, std::string title): OGUIArea(window)
{
    this->type= 1; // 3d type window
    this->title= title;
    CreateVertexBuffer();
}

} /* End of namespace OpenCVGUI */
