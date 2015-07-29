#include "skybox.h"


Skybox::Skybox(GLuint &program)
{
    this->program = program;
    this->initializeBox();
}

void Skybox::initializeBox()
{
    glGenVertexArrays(1, &skyboxVao);
    glBindVertexArray(skyboxVao);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);

    GLuint texCoorBuffer;
    glGenBuffers(1, &texCoorBuffer);

    vec3 pts[] =
    {
        // top
        vec3(1,1,1),    // 0
        vec3(1,1,-1),   // 1
        vec3(-1,1,-1),  // 2
        vec3(-1,1,1),   // 3

        // front
        vec3(1,1,1),    // 8
        vec3(-1,1,1),   // 9
        vec3(-1,-1,1),  // 10
        vec3(1,-1,1),   // 11

        // back
        vec3(-1,-1,-1), // 12
        vec3(-1,1,-1),  // 13
        vec3(1,1,-1),   // 14
        vec3(1,-1,-1),  // 15

        // right
        vec3(1,-1,1),   // 16
        vec3(1,-1,-1),  // 17
        vec3(1,1,-1),   // 18
        vec3(1,1,1),     // 19

        // left
        vec3(-1,-1,1),  // 20
        vec3(-1,1,1),   // 21
        vec3(-1,1,-1),  // 22
        vec3(-1,-1,-1), // 23

        // bottom
        vec3(1,-1,1),   // 4
        vec3(-1,-1,1),  // 5
        vec3(-1,-1,-1), // 6
        vec3(1,-1,-1)  // 7
    };

    vec2 uv[] =
    {
        // top
        vec2(1,0),
        vec2(1,1),
        vec2(0,1),
        vec2(0,0),

        // front
        vec2(1,1),
        vec2(0,1),
        vec2(0,0),
        vec2(1,0),

        // back
        vec2(1,0),
        vec2(1,1),
        vec2(0,1),
        vec2(0,0),

        // right
        vec2(0,0),
        vec2(1,0),
        vec2(1,1),
        vec2(0,1),

        // left
        vec2(1,0),
        vec2(1,1),
        vec2(0,1),
        vec2(0,0),

        // bottom
        vec2(0,1),
        vec2(0,0),
        vec2(1,0),
        vec2(1,1),
    };

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texCoorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_DYNAMIC_DRAW);

    glUseProgram(program);

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, texCoorBuffer);
    GLint uvIndex = glGetAttribLocation(program, "uv");
    glEnableVertexAttribArray(uvIndex);
    glVertexAttribPointer(uvIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);

    modelMatrixLoc = glGetUniformLocation(program, "model");

    for (int j = 0; j < 5; ++j) {
        textures[j] = new QOpenGLTexture(QImage(QString(":assets/skybox/side%1.jpg").arg(j + 1)).mirrored());
        textures[j]->setWrapMode(QOpenGLTexture::ClampToEdge);
    }
}

void Skybox::renderBox(mat4 matrix)
{
   glUseProgram(program);

   GLuint specularShininessLoc = glGetUniformLocation(program, "shininess");
   glUniform1i(specularShininessLoc, 50);

   glDepthMask(0);
   glUniformMatrix4fv(modelMatrixLoc, 1, false, value_ptr(matrix));
   glBindVertexArray(skyboxVao);

   for(int i = 0;  i < 5; i++) {
       textures[i]->bind();
       glDrawArrays(GL_TRIANGLE_FAN, i*4, 4);
   }
   glDepthMask(1);
}


