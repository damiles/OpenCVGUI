#ifndef OpenCVGUI_OGUI3D_h
#define OpenCVGUI_OGUI3D_h

#include <opencv2/core/core.hpp>
#include <GL/glew.h>
#include "OGUIArea.h"
#include <string>
#include <mutex>
#include <functional>
// Include GLM
#include <glm/glm.hpp>

using namespace cv;

namespace OpenCVGUI {

    /**
     * Set a directory with custom shaders for 3D plots.
     * If the directory contains vert.shader and/or frag.shader they replace the
     * embedded default ones; missing files keep the default shader. The
     * OPENCVGUI_SHADERS_DIR environment variable is used when no directory is set.
     * Must be called before the 3D plot is created or replotted.
     * @param dir directory with custom shaders, empty string to use only defaults
     */
    void setShadersDir(const std::string& dir);

    class OGUI3DCamera
    {
    public:
        OGUI3DCamera();
        glm::mat4 mouseEvent(int dx, int dy);
        glm::mat4 getMVP();
        glm::mat4 setRadius(float r);
        glm::mat4 Projection;
        glm::mat4 View;
        glm::mat4 Model;
        glm::mat4 MVP;
        float x,y,z;
        float phi, theta, radius;
    };

class OGUI3D : public OGUIArea {

 public:

    virtual void draw(int x, int y, int width, int height);
    virtual void draw3d(int x, int y, int width, int height);
    virtual void updateScrollStatus(double xoffset,double yoffset);
    void replot(void *data);

    /**
     * Set a function to draw custom OpenGL content in the 3D area.
     * It is called in the UI thread after the plot is drawn, with the area
     * viewport set and the camera model view projection matrix.
     * @param func function receiving area x, y, width, height and the camera MVP
     */
    void setExternalDraw3D(std::function<void(int x, int y, int width, int height, const glm::mat4& mvp)> func);

    OGUI3D(OGUIWindow* window, std::string title, void* data);

    // 3d functions
    GLuint vao, vbo, ibo, shaderProgram, MatrixID, position_attribute;
    glm::mat4 MVP;
    void CreateVertexBuffer();

    Mat data;
    int dataLength, dataLengthIBO;
    OGUI3DCamera camera;

    int lastMouseX, lastMouseY;
    bool isBufferCreated;

private:
    // Protects data, isBufferCreated and the external draw, replot can be called from any thread
    std::mutex _mutex;
    void drawPlot();
    std::function<void(int x, int y, int width, int height, const glm::mat4& mvp)> _externalDraw3D;
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUI3D_h
