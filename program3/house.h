#ifndef HOUSE_H
#define HOUSE_H

#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cube.h"
#include "texture_assets.h"
#include "grass.h"

using namespace glm;

class House {

public:
    House(GLuint &program);
    void render(mat4 matrix);

private:
    void initialize();
    void renderObject(mat4 matrix, vec3 pos[], int size);
    void bindTexture(QOpenGLTexture *texture, vec2 uv[], int size);
    void bindNormal(vec3 normal[], int size);

    void renderWall(mat4 matrix);
    void renderRoof(mat4 matrix);
    void renderDoorRoof(mat4 matrix);
    void renderDoor(mat4 matrix);
    void renderWindow(mat4 matrix);

private:
    GLuint houseVao;
    GLuint program;
    GLuint positionBuffer;
    GLuint normalBuffer;
    GLuint texCoorBuffer;

    Cube *cube;
    GLint modelMatrixLoc;

    vec3 leftWall[5];
    vec3 rightWall[5];
    vec3 frontWall[4];
    vec3 backWall[4];

    vec3 door[4];
    vec3 window[4];

    vec2 lfrgUV[5];
    vec2 ftbkUV[4];
    vec2 roofUV[4];
    vec2 doorwidowUV[4];

    vec3 leftNormal[5];
    vec3 rightNormal[5];
    vec3 frontNormal[4];
    vec3 backNormal[4];

    Grass *grass;

};

#endif // HOUSE_H

