#ifndef GROUND_H
#define GROUND_H

#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "cube.h"
#include "texture_assets.h"

using namespace glm;

class Ground {

public:
    Ground(GLuint &program);
    void render(mat4 matrix);

private:
    void initialize();

private:
    GLuint program;
    GLuint groundVAO;

    GLint modelMatrixLoc;
};



#endif // GROUND_H

