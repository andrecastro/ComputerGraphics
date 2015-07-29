#ifndef GRASS
#define GRASS

#include <QOpenGLFunctions_3_3_Core>
#include "object_loader.h"
#include "texture_assets.h"

class Grass {

public:
    Grass(GLuint &program);
    void render(mat4 matrix);

private:
    void initialize();

    ObjectLoader *loader;
    GLuint program;
};


#endif // GRASS

