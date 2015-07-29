#ifndef BARREL
#define BARREL

#include <QOpenGLFunctions_3_3_Core>
#include "object_loader.h"
#include "texture_assets.h"

class Barrel {

public:
    Barrel(GLuint &program);
    void render(mat4 matrix);

private:
    void initialize();

    ObjectLoader *loader;
    GLuint program;
};

#endif // BARREL

