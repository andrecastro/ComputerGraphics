#include "tree.h"

Tree::Tree(GLuint &program)
{
    this->loader = new ObjectLoader(program);
    this->program = program;
    this->initialize();
}

void Tree::initialize()
{
    loader->initializeModel("../program3/models/tree/Tree.obj", false);
}


void Tree::render(mat4 matrix)
{
    GLuint specularShininessLoc = glGetUniformLocation(program, "shininess");
    glUniform1i(specularShininessLoc, 50);

    TextureAssets::me().tree->bind();
    loader->renderObject(matrix);
}
