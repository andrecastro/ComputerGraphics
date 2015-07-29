#include "grass.h"

Grass::Grass(GLuint &program)
{
    this->loader = new ObjectLoader(program);
    this->program = program;
    this->initialize();
}

void Grass::initialize()
{
    loader->initializeModel("../program3/models/grass/Grass_03.obj", true);
}

void Grass::render(mat4 matrix)
{
   GLuint specularShininessLoc = glGetUniformLocation(program, "shininess");
   glUniform1i(specularShininessLoc, 50);
   TextureAssets::me().grass->bind();
   loader->renderObject(matrix);
}

