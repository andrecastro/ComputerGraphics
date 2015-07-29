#include <pyramid.h>

Pyramid::Pyramid(GLuint &program)
{
    this->program = program;
    this->initialize();
}


void Pyramid::initialize()
{
    vec3 pts[12] = {
        vec3(0.0f, 1.0f, 0.0f),
        vec3(1.0f, 0.0f, 1.0f),
        vec3(1.0f, 0.0f, -1.0f),

        vec3(0.0f, 1.0f, 0.0f),
        vec3(1.0f, 0.0f, -1.0f),
        vec3(-1.0f, 0.0f, -1.0f),

        vec3(0.0f, 1.0f, 0.0f),
        vec3(-1.0f, 0.0f, -1.0f),
        vec3(-1.0f, 0.0f,  1.0f),

        vec3(0.0f, 1.0f, 0.0f),
        vec3(-1.0f, 0.0f, 1.0f),
        vec3(1.0f, 0.0f, 1.0f)
    };

    vec3 normal[12] = {
        vec3(0.0f, 1.0f, 0.0f),
        vec3(1.0f, 0.0f, 0.0f),
        vec3(1.0f, 0.0f, 0.0f),

        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 0.0f, -1.0f),
        vec3(0.0f, 0.0f, -1.0f),

        vec3(0.0f, 1.0f, 0.0f),
        vec3(-1.0f, 0.0f, 0.0f),
        vec3(-1.0f, 0.0f,  0.0f),

        vec3(0.0f, 1.0f, 0.0f),
        vec3(0.0f, 0.0f, 1.0f),
        vec3(0.0f, 0.0f, 1.0f)
    };

    vec2 uv[12] = {
      vec2(0.5, 0.5),
      vec2(0, 0),
      vec2(0, 1),

      vec2(0.5, 0.5),
      vec2(0, 0),
      vec2(0, 1),

      vec2(0.5, 0.5),
      vec2(0, 0),
      vec2(0, 1),

      vec2(0.5, 0.5),
      vec2(0, 0),
      vec2(0, 1),
    };

    glGenVertexArrays(1, &pyramidVAO);
    glBindVertexArray(pyramidVAO);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);

    GLuint normalBuffer;
    glGenBuffers(1, &normalBuffer);

    GLuint texCoorBuffer;
    glGenBuffers(1, &texCoorBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normal), normal, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texCoorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_DYNAMIC_DRAW);

    glUseProgram(program);

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    GLint normalIndex = glGetAttribLocation(program, "normal");
    glEnableVertexAttribArray(normalIndex);
    glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, texCoorBuffer);
    GLint uvIndex = glGetAttribLocation(program, "uv");
    glEnableVertexAttribArray(uvIndex);
    glVertexAttribPointer(uvIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);

    modelMatrixLoc = glGetUniformLocation(program, "model");
}

void Pyramid::render(mat4 matrix)
{
    GLuint specularShininessLoc = glGetUniformLocation(program, "shininess");
    glUniform1i(specularShininessLoc, 128);

    glUseProgram(program);
    glUniformMatrix4fv(modelMatrixLoc, 1, false, value_ptr(matrix));
    glBindVertexArray(pyramidVAO);
    TextureAssets::me().wallBrick->bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 12);
}
