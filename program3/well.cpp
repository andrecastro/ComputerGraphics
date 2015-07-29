#include "well.h"

Well::Well(GLuint &program)
{
    this->loader = new ObjectLoader(program);
    this->program = program;
    this->initialize();
}

void Well::initialize()
{
    loader->initializeModel("../program3/models/well/Well.obj", false);
}


void Well::render(mat4 matrix)
{
    GLuint specularShininessLoc = glGetUniformLocation(program, "shininess");
    glUniform1i(specularShininessLoc, 50);

    TextureAssets::me().roof->bind();
    loader->renderObject(matrix);
}

