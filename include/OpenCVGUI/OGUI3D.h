#ifndef OpenCVGUI_OGUI3D_h
#define OpenCVGUI_OGUI3D_h

#include <opencv2/core/core.hpp>
#include <GL/glew.h>
#include "OGUIArea.h"
#include <string>
// Include GLM
#include <glm/glm.hpp>

using namespace cv;

namespace OpenCVGUI {

class OGUI3D : public OGUIArea {

 public:

    virtual void draw(int x, int y, int width, int height);
    virtual void draw3d(int x, int y, int width, int height);

    OGUI3D(OGUIWindow* window, std::string title, void* data);

    // 3d functions
    GLuint vao, vbo, shaderProgram, MatrixID, position_attribute;
    glm::mat4 MVP;
    void CreateVertexBuffer();

    Mat data;
    int dataLength;
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUI3D_h
