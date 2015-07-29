#include "cube.h"

Cube::Cube(GLuint &program)
{
    this->program = program;
    this->initializeCube();
}

void Cube::initializeCube()
{

    glGenVertexArrays(1, &cubeVao);
    glBindVertexArray(cubeVao);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);

    glGenBuffers(1, &colorBuffer);

    GLuint texCoorBuffer;
    glGenBuffers(1, &texCoorBuffer);

    GLuint normalsBuffer;
    glGenBuffers(1, &normalsBuffer);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);

    vec3 pts[] =
    {
        // top
        vec3(1,1,1),    // 0
        vec3(1,1,-1),   // 1
        vec3(-1,1,-1),  // 2
        vec3(-1,1,1),   // 3

        // bottom
        vec3(1,-1,1),   // 4
        vec3(-1,-1,1),  // 5
        vec3(-1,-1,-1), // 6
        vec3(1,-1,-1),  // 7

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
        vec3(-1,-1,-1) // 23
    };

    for(int i = 0; i < 24; i++)
    {
        pts[i] *= .5;
    }

    vec3 normals[] =
    {
        vec3(0,1,0),    // 0
        vec3(0,1,0),   // 1
        vec3(0,1,0),  // 2
        vec3(0,1,0),   // 3

        // bottom
        vec3(0,-1,0),   // 4
        vec3(0,-1,0),  // 5
        vec3(0,-1,0), // 6
        vec3(0,-1,0),  // 7

        // front
        vec3(0,0,1),    // 8
        vec3(0,0,1),   // 9
        vec3(0,0,1),  // 10
        vec3(0,0,1),   // 11

        // back
        vec3(0,0,-1), // 12
        vec3(0,0,-1),  // 13
        vec3(0,0,-1),   // 14
        vec3(0,0,-1),  // 15

        // right
        vec3(1,0,0),   // 16
        vec3(1,0,0),  // 17
        vec3(1,0,0),   // 18
        vec3(1,0,0),     // 19

        // left
        vec3(-1,0,0),  // 20
        vec3(-1,0,0),   // 21
        vec3(-1,0,0),  // 22
        vec3(-1,0,0) // 23
    };

    vec2 uv[] =
    {
        // top
        vec2(1,0),    // 0
        vec2(1,1),   // 1
        vec2(0,1),  // 2
        vec2(0,0),   // 3

        // bottom
        vec2(1,0),    // 0
        vec2(1,1),   // 1
        vec2(0,1),  // 2
        vec2(0,0),   // 3

        // front
        vec2(1,0),    // 0
        vec2(1,1),   // 1
        vec2(0,1),  // 2
        vec2(0,0),   // 3

        // back
        vec2(1,0),    // 0
        vec2(1,1),   // 1
        vec2(0,1),  // 2
        vec2(0,0),   // 3

        // right
        vec2(1,0),    // 0
        vec2(1,1),   // 1
        vec2(0,1),  // 2
        vec2(0,0),   // 3

        // left
        vec2(1,0),    // 0
        vec2(1,1),   // 1
        vec2(0,1),  // 2
        vec2(0,0),   // 3
    };

    GLuint restart = 0xFFFFFFFF;
    GLuint indices[] =
    {
        0,1,2,3, restart,
        4,5,6,7, restart,
        8,9,10,11, restart,
        12,13,14,15, restart,
        16,17,18,19, restart,
        20,21,22,23
    };

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texCoorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glUseProgram(program);

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    GLint colorIndex = glGetAttribLocation(program, "color");
    glEnableVertexAttribArray(colorIndex);
    glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, texCoorBuffer);
    GLint uvIndex = glGetAttribLocation(program, "uv");
    glEnableVertexAttribArray(uvIndex);
    glVertexAttribPointer(uvIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
    GLint normalPositionIndex = glGetAttribLocation(program, "normal");
    glEnableVertexAttribArray(normalPositionIndex);
    glVertexAttribPointer(normalPositionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    modelMatrixLoc = glGetUniformLocation(program, "model");
}

void Cube::renderCube(mat4 matrix)
{
    glUseProgram(program);
    glUniformMatrix4fv(modelMatrixLoc, 1, false, value_ptr(matrix));
    glBindVertexArray(cubeVao);
    glDrawElements(GL_TRIANGLE_FAN, 29, GL_UNSIGNED_INT, 0);
}

void Cube::bindTextureImage(QOpenGLTexture *texture)
{
    this->bindColor(vec3(0, 0, 0));
    texture->bind();
}

void Cube::bindColor(vec3 color)
{
    bindColor(color, color, color, color, color, color);
}

void Cube::bindColor(vec3 top, vec3 bottom, vec3 right, vec3 left, vec3 front, vec3 back)
{
    vec3 colors[] = {
        /* top color */ top, top, top, top,
        /* bottom color */ bottom, bottom, bottom, bottom,
        /* front color */ front, front, front, front,
        /* back color */ back, back, back, back,
        /* right color */ right, right, right, right,
        /* left color */ left, left, left, left
    };

    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_DYNAMIC_DRAW);
}
