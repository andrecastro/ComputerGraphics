#include "barrel.h"

Barrel::Barrel(GLuint &program)
{
    this->loader = new ObjectLoader(program);
    this->program = program;
    this->initialize();
}

void Barrel::initialize()
{
    loader->initializeModel("../program3/models/barrel/MedievalBarrel_OBJ.obj", true);
}


void Barrel::render(mat4 matrix)
{
    GLuint specularShininessLoc = glGetUniformLocation(program, "shininess");
    glUniform1i(specularShininessLoc, 50);
    TextureAssets::me().barrel->bind();
    loader->renderObject(matrix);
}

