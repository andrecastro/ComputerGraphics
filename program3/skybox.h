#ifndef SKYBOX_H
#define SKYBOX_H

#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QOpenGLTexture>

using namespace glm;

class Skybox {

public:
    Skybox(GLuint &program);
    void renderBox(mat4 matrix);

private:
    void initializeBox();

private:
    GLuint program;
    GLuint skyboxVao;

    QOpenGLTexture *textures[5];
    GLint modelMatrixLoc;
};

#endif // SKYBOX_H

