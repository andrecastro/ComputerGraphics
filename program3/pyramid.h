#ifndef PYRAMID
#define PYRAMID

#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "texture_assets.h"

using namespace glm;

class Pyramid {

public:
    Pyramid(GLuint &program);
    void render(mat4 matrix);

private:
    void initialize();

    GLuint program;
    GLuint pyramidVAO;

    GLint modelMatrixLoc;
};


#endif // PYRAMID

