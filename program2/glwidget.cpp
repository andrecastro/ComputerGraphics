#include "glwidget.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QTextStream>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) { 
    brickWidth = .8f; // 1 to 0.1 default .8
    brickHeight = .2f; // 1 to 0.1 default .2
    brickDepth = .8f; // 1 to 0.1 default .8
    space = .03f; // .1 to .03 default .03
    wallSize = 10.0f; // 1 to 10 default 10
    constant = 0.02f;
    rowType = true;
}

GLWidget::~GLWidget() {

}

// initialize the grid coordinates
void GLWidget::initializeGrid() {
    glGenVertexArrays(1, &gridVao);
    glBindVertexArray(gridVao);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);

    vec3 pts[84];
    for(int i = -10; i <= 10; i++) {

        pts[2*(i+10)] = vec3(i, -.5f, 10);
        pts[2*(i+10)+1] = vec3(i, -.5f, -10);

        pts[2*(i+10)+42] = vec3(10,-.5f, i);
        pts[2*(i+10)+43] = vec3(-10,-.5f, i);
    }

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pts), pts, GL_STATIC_DRAW);

    GLuint program = loadShaders(":/grid_vert.glsl", ":/grid_frag.glsl");
    glUseProgram(program);
    gridProg = program;

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    GLint positionIndex = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(positionIndex);
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

    gridProjMatrixLoc = glGetUniformLocation(program, "projection");
    gridViewMatrixLoc = glGetUniformLocation(program, "view");
    gridModelMatrixLoc = glGetUniformLocation(program, "model");
}

// initialize the cube coordinates, color,
// and texture coordinates
void GLWidget::initializeCube() {
    glGenVertexArrays(1, &brickVao);
    glBindVertexArray(brickVao);

    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);

    glGenBuffers(1, &colorBuffer);

    GLuint texCoorBuffer;
    glGenBuffers(1, &texCoorBuffer);

    GLuint normalsBuffer;
    glGenBuffers(1, &normalsBuffer);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);

    vec3 pts[] = {
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

    for(int i = 0; i < 24; i++) {
        pts[i] *= .5;
    }

    vec3 normals[] = {
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

    vec2 uv[] = {
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
    GLuint indices[] = {
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

    // Load our vertex and fragment shaders into a program object
    // on the GPU
    GLuint program = loadShaders(":/vert.glsl", ":/frag.glsl");
    glUseProgram(program);
    wallProg = program;

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

    wallProjecMatrixLoc = glGetUniformLocation(program, "projection");
    wallViewMatrixLoc = glGetUniformLocation(program, "view");
    wallModelMatrixLoc = glGetUniformLocation(program, "model");

    enableDiffuseLoc = glGetUniformLocation(program, "enableDiffuse");
    glUniform1i(enableDiffuseLoc, GL_FALSE);

    enableSpecularLoc = glGetUniformLocation(program, "enableSpecular");
    glUniform1i(enableSpecularLoc, GL_FALSE);

    enableTextureLoc = glGetUniformLocation(program, "enableTexture");
    glUniform1i(enableTextureLoc, GL_FALSE);

    specularShininessLoc = glGetUniformLocation(program, "shininess");
    glUniform1i(specularShininessLoc, 128);

    GLint lightPosition1Index = glGetUniformLocation(program, "lightPosition");
    glUniform3f(lightPosition1Index, 0.0f, 100.0f, 0.0f);
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);

    glEnable(GL_DEPTH_TEST);
    GLuint restart = 0xFFFFFFFF;
    glPrimitiveRestartIndex(restart);
    glEnable(GL_PRIMITIVE_RESTART);

    initializeGrid();
    initializeCube();

    viewDist = -20.0f;

    glUseProgram(wallProg);
    GLint eyePositionLoc = glGetUniformLocation(wallProg, "eyePosition");
    glUniform3f(eyePositionLoc, 0.0f, 0.0f, viewDist);

    viewMatrix = lookAt(vec3(0.0f, 0.0f, viewDist), vec3(0,0,0), vec3(0,1,0));
    wallModelMatrix = mat4(1.0f);
    gridModelMatrix = mat4(1.0f);
}

void GLWidget::resizeGL(int w, int h) {
    width = w;
    height = h;

    float aspect = (float)w/h;

    projMatrix = perspective(45.0f, aspect, 1.0f, 100.0f);

    glUseProgram(wallProg);
    glUniformMatrix4fv(wallProjecMatrixLoc, 1, false, value_ptr(projMatrix));

    glUseProgram(gridProg);
    glUniformMatrix4fv(gridProjMatrixLoc, 1, false, value_ptr(projMatrix));
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    wallHeight = ((brickHeight + space) * wallSize) - space - constant;
    wallWidth = ((brickWidth + space) * wallSize) - space - constant;
    wallDepth = brickDepth - .1f;

    renderGrid();

    // render single wall or square
    if(rowType) {
        renderWall(wallModelMatrix);
    } else {
        renderSquareWall();
    }
}

// render the grid
void GLWidget::renderGrid() {
    glUseProgram(gridProg);
    glUniformMatrix4fv(gridViewMatrixLoc, 1, false, value_ptr(viewMatrix));
    glUniformMatrix4fv(gridModelMatrixLoc, 1, false, value_ptr(gridModelMatrix));
    glBindVertexArray(gridVao);
    glDrawArrays(GL_LINES, 0, 84);
}

// render a square wall
void GLWidget::renderSquareWall() {
    mat4 front = wallModelMatrix * translate(mat4(), vec3(0, 0, -wallWidth/2 - brickDepth/2));
    renderWall(front);

    mat4 back = front * translate(mat4(), vec3(0, 0, wallWidth + brickDepth));
    renderWall(back);

    mat4 left = front * rotate(mat4(), (float)M_PI/2, vec3(0, 1, 0)) * translate(mat4(), vec3(-wallWidth/2 - brickDepth/2, 0, wallWidth/2 - brickDepth/2));
    renderWall(left);

    mat4 right = left * translate(mat4(), vec3(0, 0, -wallWidth + brickDepth));
    renderWall(right);
}

// render a single wall
void GLWidget::renderWall(mat4 matrix) {
    bindColor(vec3(.62f));

    mat4 model =  matrix * scale(mat4(), vec3(wallWidth, wallHeight, wallDepth));
    renderCube(model);
    renderBrick(matrix);
}

// render all bricks of a wall
void GLWidget::renderBrick(mat4 baseMatrix) {
    float maxHei = wallHeight * (0.5f);
    float maxWid = wallWidth * (0.5f);
    float halfBrickW = brickWidth/2;
    float halfBrickH = brickHeight/2;
    float halfConstant = constant/2;

    bindColor(vec3(0.64f, .16f, .16f));
    bindTextureImage(QImage(":/brick.jpg"));

    mat4 scaleMat = scale(mat4(), vec3(brickWidth, brickHeight, brickDepth));
    mat4 model;

    for(int i = 0; i < wallSize; i++) {
        for(int j = 0; j < wallSize; j++) {
            model = baseMatrix * translate(mat4(), vec3(maxWid - ((2*i + 1) * halfBrickW + i * space) + halfConstant,
                                                             maxHei - ((2*j + 1) * halfBrickH + j * space) + halfConstant,
                                                             0.0f));
            renderCube(model * scaleMat);
        }
    }
}

// render a cube
void GLWidget::renderCube(mat4 matrix) {
    glUseProgram(wallProg);
    glUniformMatrix4fv(wallViewMatrixLoc, 1, false, value_ptr(viewMatrix));
    glUniformMatrix4fv(wallModelMatrixLoc, 1, false, value_ptr(matrix));
    glBindVertexArray(brickVao);
    glDrawElements(GL_TRIANGLE_FAN, 29, GL_UNSIGNED_INT, 0);
}

/* trackball */
void GLWidget::mousePressEvent(QMouseEvent *event) {
    vec2 pt(event->x(), event->y());
    lastVPt = normalize(pointOnVirtualTrackball(pt));
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    vec2 pt(event->x(), event->y());
    vec3 vPt = normalize(pointOnVirtualTrackball(pt));
    vec3 axis = cross(lastVPt, vPt);

    if(length(axis) >= .00001) {
        axis = normalize(axis);
        float angle = acos(dot(vPt,lastVPt));
        mat4 r = rotate(mat4(1.0f), angle, axis);

        gridModelMatrix = r * gridModelMatrix;
        wallModelMatrix =  r * wallModelMatrix;
    }

    lastVPt = vPt;
    update();
}

vec3 GLWidget::pointOnVirtualTrackball(const vec2 &pt) {
    float r = .5f;
    float rr = r*r;
    vec3 p;
    p.x = -1 + pt.x*(2.0f/width);
    p.y = -(float)height/width*(1-pt.y*(2.0f/height));

    float xx = p.x*p.x;
    float yy = p.y*p.y;

    if(xx+yy <= rr*.5) {
        p.z = sqrt(rr-(xx+yy));
    } else {
        p.z = rr*.5/sqrt(xx+yy);
    }

    return p;
}
/* trackball */

/* zoom */
void GLWidget::wheelEvent(QWheelEvent *event) {
    makeCurrent();
    viewDist += event->delta();

    if (viewDist > -5.0f) {
        viewDist = -5.0f;
    }

    viewMatrix = lookAt(vec3(0,0,viewDist),vec3(0,0,0),vec3(0,1,0));

    glUseProgram(wallProg);
    glUniformMatrix4fv(wallViewMatrixLoc, 1, false, value_ptr(viewMatrix));

    glUseProgram(gridProg);
    glUniformMatrix4fv(gridViewMatrixLoc, 1, false, value_ptr(viewMatrix));

    update();
}
/* zoom */


// bind a texture
void GLWidget::bindTextureImage(QImage image) {
    tex = new QOpenGLTexture(image.mirrored());
    tex->bind();
}

// bind a color
void GLWidget::bindColor(vec3 color) {
    bindColor(color, color, color, color, color, color);
}

// bind a color
void GLWidget::bindColor(vec3 top, vec3 bottom, vec3 right, vec3 left, vec3 front, vec3 back) {
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

// listener to change the wall size
void GLWidget::changeWallSize(int newSize) {
    wallSize = (float) newSize;
    update();
}

// listener to change the brick space
void GLWidget::changeBrickSpace(int newSpace) {
    space = (float) newSpace/100;
    update();
}

// listener to change the brick width
void GLWidget::changeBrickWidth(int newWidth) {
    brickWidth = (float) newWidth/10;
    update();
}

// listener to change the brick heigth
void GLWidget::changeBrickHeight(int newHeight) {
    brickHeight = (float) newHeight/10;
    update();
}

// listener to change the brick depth
void GLWidget::changeBrickDepth(int newDepth) {
    brickDepth = (float) newDepth/10;
    update();
}

// listener to enable/disable diffuse
void GLWidget::toggleDiffuse(bool diffuse) {
    glUseProgram(wallProg);
    glUniform1i(enableDiffuseLoc, diffuse);
    update();
}

// listener to enable/disable specular
void GLWidget::toggleSpecular(bool specular) {
    glUseProgram(wallProg);
    glUniform1i(enableSpecularLoc, specular);
    update();
}

// listener to enable/disable texture
void GLWidget::toggleTexture(bool texture) {
    glUseProgram(wallProg);
    glUniform1i(enableTextureLoc, texture);
    update();
}

// listener to change the type of wall to be rendered
void GLWidget::toggleType(bool type) {
    rowType = type;
    update();
}

// listener to chnage the shininess
void GLWidget::changeSpecularShininess(int shininess) {
    glUseProgram(wallProg);
    glUniform1i(specularShininessLoc, shininess);
    update();
}
