#include "ground.h"
#include <glm/gtc/matrix_transform.hpp>

Ground::Ground(GLuint &program)
{
    this->program = program;
    this->initialize();
}

void Ground::initialize()
{
    vec3 pts[4] = {
        vec3(10.0f, -2.0f, 10.0f),
        vec3(-10.0f, -2.0f, 10.0f),
        vec3(-10.0f, -2.0f, -10.0f),
        vec3(10.0f, -2.0f, -10.0f)
    };

    vec2 uv[4] = {
      vec2(1, 0),
      vec2(0, 0),
      vec2(0, 1),
      vec2(1, 1)
    };

    glGenVertexArrays(1, &groundVAO);
    glBindVertexArray(groundVAO);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);

    GLuint texCoorBuffer;
    glGenBuffers(1, &texCoorBuffer);

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
}

void Ground::render(mat4 matrix)
{
    glUseProgram(program);

    GLuint specularShininessLoc = glGetUniformLocation(program, "shininess");
    glUniform1i(specularShininessLoc, 50);

    for(int i = -50; i < 50; i++) {
        for(int j = -50; j < 50; j++) {
            glUniformMatrix4fv(modelMatrixLoc, 1, false, value_ptr(matrix * glm::translate(mat4(), vec3(i * 20.0, 0.0, j * 20))));
            glBindVertexArray(groundVAO);
            TextureAssets::me().ground->bind();
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        }
    }


}
