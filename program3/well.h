#ifndef WELL
#define WELL


#include <QOpenGLFunctions_3_3_Core>
#include "object_loader.h"
#include "texture_assets.h"

class Well {

public:
    Well(GLuint &program);
    void render(mat4 matrix);

private:
    void initialize();

    ObjectLoader *loader;
    GLuint program;
};

#endif // WELL

