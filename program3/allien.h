#ifndef ALLIEN
#define ALLIEN

#include <QOpenGLFunctions_3_3_Core>
#include "object_loader.h"
#include "texture_assets.h"

class Allien {

public:
    Allien(GLuint &program);
    void render(mat4 matrix);

private:
    void initialize();

    ObjectLoader *loader;

    GLuint program;
};

#endif // ALLIEN

