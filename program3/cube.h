#ifndef CUBE_H
#define CUBE_H

#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QOpenGLTexture>

using namespace glm;

class Cube {

public:
    Cube(GLuint &program);
    void renderCube(mat4 matrix);

    void bindTextureImage(QOpenGLTexture *image);
    void bindColor(vec3 color);
    void bindColor(vec3 top, vec3 bottom, vec3 right, vec3 left, vec3 front, vec3 back);

private:
    void initializeCube();

private:
    GLuint program;
    GLuint cubeVao;
    GLuint colorBuffer;

    GLint modelMatrixLoc;
};

#endif // CUBE_H

