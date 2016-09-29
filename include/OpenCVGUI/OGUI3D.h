#ifndef OpenCVGUI_OGUI3D_h
#define OpenCVGUI_OGUI3D_h

#include <GL/glew.h>
#include "OGUIArea.h"
#include <string>
// Include GLM
#include <glm/glm.hpp>

namespace OpenCVGUI {

    struct Vector3f
    {
        float x;
        float y;
        float z;

        Vector3f() {}

        Vector3f(float _x, float _y, float _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        Vector3f(const float* pFloat)
        {
            x = pFloat[0];
            y = pFloat[0];
            z = pFloat[0];
        }

        Vector3f(float f)
        {
            x = y = z = f;
        }

        Vector3f& operator+=(const Vector3f& r)
        {
            x += r.x;
            y += r.y;
            z += r.z;

            return *this;
        }

        Vector3f& operator-=(const Vector3f& r)
        {
            x -= r.x;
            y -= r.y;
            z -= r.z;

            return *this;
        }

        Vector3f& operator*=(float f)
        {
            x *= f;
            y *= f;
            z *= f;

            return *this;
        }

        operator const float*() const
        {
            return &(x);
        }


        void Print() const
        {
            printf("(%.02f, %.02f, %.02f)", x, y, z);
        }
    };


class OGUI3D : public OGUIArea {

 public:

    virtual void draw(int x, int y, int width, int height);
    virtual void draw3d(int x, int y, int width, int height);

    OGUI3D(OGUIWindow* window, std::string title);

    // 3d functions
    GLuint vao, vbo, shaderProgram, MatrixID, position_attribute;
    glm::mat4 MVP;
    void CreateVertexBuffer();
};

} /* End of namespace OpenCVGUI */

#endif // OpenCVGUI_OGUI3D_h
