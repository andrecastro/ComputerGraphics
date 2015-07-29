#include "house.h"
#include <glm/gtc/matrix_transform.hpp>

House::House(GLuint &program)
{
    this->program = program;
    this->initialize();
}

void House::initialize()
{
    leftWall[0] = vec3(-2, 1.25, 1);
    leftWall[1] = vec3(-2, 2.2, 0);
    leftWall[2] = vec3(-2, 1.25, -1);
    leftWall[3] = vec3(-2, 0, -1);
    leftWall[4] =  vec3(-2, 0, 1);

    leftNormal[0] = vec3(-1, 0, 0);
    leftNormal[1] = vec3(-1, 0, 0);
    leftNormal[2] = vec3(-1, 0, 0);
    leftNormal[3] = vec3(-1, 0, 0);
    leftNormal[4] =  vec3(-1, 0, 0);

    rightWall[0] = vec3(2, 1.25, -1);
    rightWall[1] = vec3(2, 2.2, 0);
    rightWall[2] = vec3(2, 1.25, 1);
    rightWall[3] = vec3(2, 0, 1);
    rightWall[4] = vec3(2, 0, -1);

    rightNormal[0] = vec3(1, 0, 0);
    rightNormal[1] = vec3(1, 0, 0);
    rightNormal[2] = vec3(1, 0, 0);
    rightNormal[3] = vec3(1, 0, 0);
    rightNormal[4] =  vec3(1, 0, 0);

    frontWall[0] = vec3(2, 0, 1);
    frontWall[1] = vec3(2, 1.25, 1);
    frontWall[2] = vec3(-2, 1.25, 1);
    frontWall[3] = vec3(-2, 0, 1);

    frontNormal[0] = vec3(0, 0, 1);
    frontNormal[1] = vec3(0, 0, 1);
    frontNormal[2] = vec3(0, 0, 1);
    frontNormal[3] = vec3(0, 0, 1);

    backWall[0] = vec3(-2, 0, -1);
    backWall[1] = vec3(-2, 1.25, -1);
    backWall[2] = vec3(2, 1.25, -1);
    backWall[3] = vec3(2, 0, -1);

    backNormal[0] = vec3(0, 0, -1);
    backNormal[1] = vec3(0, 0, -1);
    backNormal[2] = vec3(0, 0, -1);
    backNormal[3] = vec3(0, 0, -1);

    door[0] = vec3(1.5f , 0.0f, 1.07f);
    door[1] = vec3(1.5f , 0.8f, 1.07f);
    door[2] = vec3(1.0f , 0.8f, 1.07f);
    door[3] = vec3(1.0f , 0.0f, 1.07f);

    window[0] = vec3(0.5f , 0.4f, 1.02f);
    window[1] = vec3(0.5f , 0.8f, 1.02f);
    window[2] = vec3(0.0f , 0.8f, 1.02f);
    window[3] = vec3(0.0f , 0.4f, 1.02f);

    lfrgUV[0] = vec2(1.0f, 0.0f);
    lfrgUV[1] = vec2(0.5f, 0.5f);
    lfrgUV[2] = vec2(0.0f, 0.0f);
    lfrgUV[3] = vec2(0.0f, 1.0f);
    lfrgUV[4] = vec2(1.0f, 1.0f);

    ftbkUV[0] = vec2(0, 0);
    ftbkUV[1] = vec2(0, 1);
    ftbkUV[2] = vec2(1, 1);
    ftbkUV[3] = vec2(1, 0);

    doorwidowUV[0] = vec2(0, 1);
    doorwidowUV[1] = vec2(0 , 0);
    doorwidowUV[2] = vec2(1, 0);
    doorwidowUV[3] = vec2(1, 1);

    glGenVertexArrays(1, &houseVao);
    glBindVertexArray(houseVao);
    glGenBuffers(1, &positionBuffer);
    glGenBuffers(1, &texCoorBuffer);
    glGenBuffers(1, &normalBuffer);

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
    grass = new Grass(program);
    cube = new Cube(program);
}

void House::render(mat4 matrix)
{
    for(int i = -2; i < 2; i++) {
        for(int j = -8; j < 8; j++) {
            grass->render(matrix
                          * glm::scale(mat4(), vec3(0.2, .03, 0.2))
                          * glm::translate(mat4(), vec3(i * 2.0f, 0.0f, j*2)));
        }
    }

    renderWall(matrix);
    renderRoof(matrix);
    renderDoorRoof(matrix);
    renderDoor(matrix);

    renderWindow(matrix);
    renderWindow(matrix * glm::translate(mat4(), vec3(-0.8f, 0.0f, 0.0f)));
    renderWindow(matrix * glm::translate(mat4(), vec3(-1.6f, 0.0f, 0.0f)));

    renderWindow(matrix * glm::rotate(mat4(), 3.141f, vec3(0.0f, 1.0f, 0.0f)));

    renderWindow(matrix * glm::translate(mat4(), vec3(0.8f, 0.0f, 0.0f))
                        * glm::rotate(mat4(), 3.141f, vec3(0.0f, 1.0f, 0.0f)));

    renderWindow(matrix * glm::translate(mat4(), vec3(1.6f, 0.0f, 0.0f))
                        * glm::rotate(mat4(), 3.141f, vec3(0.0f, 1.0f, 0.0f)));

    renderWindow(matrix * glm::translate(mat4(), vec3(-0.8f, 0.0f, 0.0f))
                        * glm::rotate(mat4(), 3.141f, vec3(0.0f, 1.0f, 0.0f)));
}

void House::renderWall(mat4 matrix)
{
    // render wall detail
    cube->bindTextureImage(TextureAssets::me().wall);
    cube->renderCube(matrix * scale(mat4(), vec3(4.07f, 0.5f, 2.07f)));

    // render left wall
    bindNormal(leftNormal, 5);
    bindTexture(TextureAssets::me().wallBrick, lfrgUV, 5);
    renderObject(matrix, leftWall, 5);

    // render right wall
    bindNormal(rightNormal, 5);
    bindTexture(TextureAssets::me().wallBrick, lfrgUV, 5);
    renderObject(matrix, rightWall, 5);

    // render front wall
    bindNormal(frontNormal, 4);
    bindTexture(TextureAssets::me().wallBrick, ftbkUV, 4);
    renderObject(matrix, frontWall, 4);

    // render back wall
    bindNormal(backNormal, 4);
    bindTexture(TextureAssets::me().wallBrick, ftbkUV, 4);
    renderObject(matrix, backWall, 4);
}

void House::renderRoof(mat4 matrix)
{
    cube->bindTextureImage(TextureAssets::me().roof);
    cube->renderCube(matrix * translate(mat4(), vec3(0.0f, 1.68f, .55f))
                            * rotate(mat4(), 0.785f, vec3(1, 0 ,0))
                            * scale(mat4(), vec3(4.2f, .05f , 1.55f)));

    cube->renderCube(matrix * translate(mat4(), vec3(0.0f, 1.68f, -.55f))
                            * rotate(mat4(), -0.785f, vec3(1, 0 ,0))
                            * scale(mat4(), vec3(4.2f, .05f , 1.60f)));
}

void House::renderDoorRoof(mat4 matrix)
{
    cube->bindTextureImage(TextureAssets::me().roof);
    cube->renderCube(matrix * translate(mat4(), vec3(1.25f, 1.25f, 1.75f))
                            * scale(mat4(), vec3(1.5f, .05f, 1.5f)));

    // render pilaster right and left
    cube->bindTextureImage(TextureAssets::me().wall);
    cube->renderCube(matrix * translate(mat4(), vec3(1.965f, 0.625f, 2.465f))
                            * scale(mat4(), vec3(.05f, 1.25f, .05f)));

    cube->renderCube(matrix * translate(mat4(), vec3(0.535f, 0.625f, 2.465f))
                            * scale(mat4(), vec3(.05f, 1.25f, .05f)));

    // render wall plaster
    cube->bindTextureImage(TextureAssets::me().wall);
    cube->renderCube(matrix * translate(mat4(), vec3(1.25f, 0.0f, 2.465f))
                            * rotate(mat4(), 3.141f, vec3(0, 1, 0))
                            * scale(mat4(), vec3(1.5f, 1.0f, 0.1f)));

    cube->renderCube(matrix * translate(mat4(), vec3(1.95f, 0.0f, 1.75f))
                            * rotate(mat4(), 3.141f, vec3(0, 1, 0))
                            * rotate(mat4(), 1.57f, vec3(0, 1, 0))
                            * scale(mat4(), vec3(1.5f, 1.0f, 0.1f)));

    // render pilaster thicker
    cube->bindTextureImage(TextureAssets::me().wall);
    cube->renderCube(matrix * translate(mat4(), vec3(1.965f, 0.0f, 2.465f))
                            * scale(mat4(), vec3(.2f, 1.1f, .2f)));

    cube->renderCube(matrix * translate(mat4(), vec3(0.535f, 0.0f, 2.465f))
                            * scale(mat4(), vec3(.2f, 1.1f, .2f)));
}

void House::renderDoor(mat4 matrix)
{
    cube->bindTextureImage(TextureAssets::me().wall);
    cube->renderCube(matrix * translate(mat4(), vec3(1.5f, 0.4f, 1.02f))
                            * scale(mat4(), vec3(.02f, 0.83f, .12f)));

    cube->renderCube(matrix * translate(mat4(), vec3(1.0f, 0.4f, 1.02f))
                            * scale(mat4(), vec3(.02f, 0.82f, .12f)));

    cube->renderCube(matrix * translate(mat4(), vec3(1.25f, 0.82f, 1.02f))
                            * scale(mat4(), vec3(.52f, 0.02f, .13f)));

    bindTexture(TextureAssets::me().door, doorwidowUV, 4);
    renderObject(matrix, door, 4);
}

void House::renderWindow(mat4 matrix)
{
    cube->bindTextureImage(TextureAssets::me().wall);
    cube->renderCube(matrix * translate(mat4(), vec3(0.5f, 0.6f, 0.98f))
                            * scale(mat4(), vec3(.02f, 0.43f, .12f)));

    cube->renderCube(matrix * translate(mat4(), vec3(0.0f, 0.6f, 0.98f))
                            * scale(mat4(), vec3(.02f, 0.43f, .12f)));

    cube->renderCube(matrix * translate(mat4(), vec3(0.25f, 0.82f, 0.98f))
                            * scale(mat4(), vec3(.52f, 0.02f, .13f)));

    cube->renderCube(matrix * translate(mat4(), vec3(0.25f, 0.39f, 0.98f))
                            * scale(mat4(), vec3(.52f, 0.02f, .13f)));

    bindTexture(TextureAssets::me().window, doorwidowUV, 4);
    renderObject(matrix, window, 4);
}

void House::renderObject(mat4 matrix, vec3 pos[], int size)
{
    GLuint specularShininessLoc = glGetUniformLocation(program, "shininess");
    glUniform1i(specularShininessLoc, 50);

    glUseProgram(program);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * size, pos, GL_STATIC_DRAW);
    glUniformMatrix4fv(modelMatrixLoc, 1, false, value_ptr(matrix));
    glBindVertexArray(houseVao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, size);
}

void House::bindTexture(QOpenGLTexture *texture, vec2 uv[], int size)
{
    glBindBuffer(GL_ARRAY_BUFFER, texCoorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * size, uv, GL_DYNAMIC_DRAW);
    texture->bind();
}

void House::bindNormal(vec3 normal[], int size)
{
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * size, normal, GL_DYNAMIC_DRAW);
}
