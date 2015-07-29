#ifndef TREE
#define TREE

#include <QOpenGLFunctions_3_3_Core>
#include "object_loader.h"
#include "texture_assets.h"

class Tree {

public:
    Tree(GLuint &program);
    void render(mat4 matrix);

private:
    void initialize();

    ObjectLoader *loader;
    GLuint program;
};

#endif // TREE

