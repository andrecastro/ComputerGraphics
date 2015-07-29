#include "allien.h"


Allien::Allien(GLuint &program)
{
    this->loader = new ObjectLoader(program);
    this->program = program;
    this->initialize();
}

void Allien::initialize()
{
    loader->initializeModel("../program3/models/allien/alinen_high.obj", true);
}


void Allien::render(mat4 matrix)
{
    GLuint specularShininessLoc = glGetUniformLocation(program, "shininess");
    glUniform1i(specularShininessLoc, 128);

    TextureAssets::me().grass->bind();
    loader->renderObject(matrix);
}
