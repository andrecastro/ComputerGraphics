#include "glwidget.h"
#include <iostream>
#include <QOpenGLTexture>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QTextStream>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

using namespace std;

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    pitch = 0.0f;
    yaw = 0.0f;
    distance = 0.1f;
    time = 0.16f;
    animationTime = 0;

    lightIntensity = 2.0f;
    gravity = false;
    specularEnable = true;
    diffuseEnable = true;

    forwardVector = vec3(0.0f, 0.0f, 1.0f);
    rightVector = vec3(1.0f, 0.0f, 0.0f);

    rotations.push_back(vec3(0, 0, 0));
    rotations.push_back(vec3(0, -4*M_PI, -4*M_PI));
    rotations.push_back(vec3(0, -4*M_PI, -4*M_PI));


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(animate()));
    timer->start(5);
}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glPointSize(4.0f);

    glEnable(GL_DEPTH_TEST);
    GLuint restart = 0xFFFFFFFF;
    glPrimitiveRestartIndex(restart);
    glEnable(GL_PRIMITIVE_RESTART);

    program = loadShaders(":/vert.glsl", ":/frag.glsl");
    glUseProgram(program);

    cube = new Cube(program);
    skybox = new Skybox(program);
    ground = new Ground(program);
    house = new House(program);
    pyramid = new Pyramid(program);
    tree = new Tree(program);
    grass = new Grass(program);
    well = new Well(program);
    barrel = new Barrel(program);
    allien = new Allien(program);

    projMatrixLoc = glGetUniformLocation(program, "projection");
    viewMatrixLoc = glGetUniformLocation(program, "view");
    modelMatrixLoc = glGetUniformLocation(program, "model");

    viewMatrix = mat4(1.0f);
    modelMatrix = mat4(1.0f);

    glUseProgram(program);
    glUniformMatrix4fv(viewMatrixLoc, 1, false, value_ptr(viewMatrix));
    glUniformMatrix4fv(modelMatrixLoc, 1, false, value_ptr(modelMatrix));

    lightPosition = vec3(0.0f, 100.0f, 0.0f);

    yRotationAnimation = mat4();
    zRotationAnimation = mat4();
}

void GLWidget::resizeGL(int w, int h)
{
    width = w;
    height = h;

    float aspect = (float)w/h;

    projMatrix = perspective(45.0f, aspect, .01f, 100000.0f);

    glUseProgram(program);
    glUniformMatrix4fv(projMatrixLoc, 1, false, value_ptr(projMatrix));
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLint lightPositionIndex = glGetUniformLocation(program, "lightPosition");
    glUniform3f(lightPositionIndex, lightPosition.x, lightPosition.y, lightPosition.z);

    GLint lightIntensityIndex = glGetUniformLocation(program, "lightIntensity");
    glUniform1f(lightIntensityIndex, lightIntensity);

    GLint enableDiffuseLoc = glGetUniformLocation(program, "enableDiffuse");
    glUniform1i(enableDiffuseLoc, diffuseEnable);

    GLint enableSpecularLoc = glGetUniformLocation(program, "enableSpecular");
    glUniform1i(enableSpecularLoc, specularEnable);

    // skybox and ground
    skybox->renderBox(modelMatrix * glm::scale(mat4(), vec3(1000, 500, 1000)));
    ground->render(modelMatrix);

    // house scene
    house->render(modelMatrix * glm::translate(mat4(), vec3(0.0f, -2.0f, -50.0f))
                              * glm::scale(mat4(), vec3(5, 5, 5)));

    house->render(modelMatrix * glm::rotate(mat4(), 0.785f , vec3(0.0f, 1.0f, 0.0f))
                              * glm::translate(mat4(), vec3(-25.0f, -2.0f, -50.0f))
                              * glm::scale(mat4(), vec3(5, 5, 5)));

    house->render(modelMatrix * glm::rotate(mat4(), -0.785f , vec3(0.0f, 1.0f, 0.0f))
                              * glm::translate(mat4(), vec3(25.0f, -2.0f, -50.0f))
                              * glm::scale(mat4(), vec3(5, 5, 5)));

    tree->render(modelMatrix * glm::translate(mat4(), vec3(-50.0f, -2.0f, 0.0f))
                             * glm::scale(mat4(), vec3(0.08, 0.08, 0.08)));

    tree->render(modelMatrix * glm::translate(mat4(), vec3(30.0f, -2.0f, 0.0f))
                             * glm::scale(mat4(), vec3(0.08, 0.08, 0.08)));


    allien->render(modelMatrix  * glm::translate(mat4(), vec3(00.0f, 100.0f, 0.0f))
                                * yRotationAnimation
                                * glm::rotate(mat4(), -1.570f, vec3(1.0f, 0.0f, 0.0f)));

    allien->render(modelMatrix  * glm::translate(mat4(), vec3(-10.0f, 100.0f,-10.0f))
                                * yRotationAnimation
                                * glm::rotate(mat4(), -1.570f, vec3(1.0f, 0.0f, 0.0f)));

    allien->render(modelMatrix  * glm::translate(mat4(), vec3(10.0f, 100.0f, -10.0f))
                                * yRotationAnimation
                                * glm::rotate(mat4(), -1.570f, vec3(1.0f, 0.0f, 0.0f)));

    well->render(modelMatrix * glm::translate(mat4(), vec3(5.0f, -2.0f, -5.0f))
                             * glm::scale(mat4(), vec3(0.015, 0.015, 0.015)));

    barrel->render(modelMatrix * glm::translate(mat4(), vec3(-1.0f, -2.0f, -5.0f))
                               * glm::scale(mat4(), vec3(0.5, 0.5, 0.5)));

    barrel->render(modelMatrix * glm::translate(mat4(), vec3(10.0f, -2.0f, -5.0f))
                               * glm::scale(mat4(), vec3(0.5, 0.5, 0.5)));

    // pyramed scene
    pyramid->render(modelMatrix * glm::translate(mat4(), vec3(0.0f, -2.0f, -200.0f))
                                * glm::scale(mat4(), vec3(10, 10, 10)));

    pyramid->render(modelMatrix * glm::rotate(mat4(), 0.785f , vec3(0.0f, 1.0f, 0.0f))
                                * glm::translate(mat4(), vec3(0.0f, -2.0f, -200.0f))
                                * glm::scale(mat4(), vec3(10, 10, 10)));

    pyramid->render(modelMatrix * glm::rotate(mat4(), -0.785f , vec3(0.0f, 1.0f, 0.0f))
                                * glm::translate(mat4(), vec3(0.0f, -2.0f, -200.0f))
                                * glm::scale(mat4(), vec3(10, 10, 10)));

    allien->render(modelMatrix  * glm::translate(mat4(), vec3(00.0f, -2.0f, -180.0f))
                                * zRotationAnimation
                                * glm::rotate(mat4(), -1.570f, vec3(1.0f, 0.0f, 0.0f)));

    allien->render(modelMatrix  * glm::translate(mat4(), vec3(-10.0f, -2.0f,-170.0f))
                                * zRotationAnimation
                                * glm::rotate(mat4(), -1.570f, vec3(1.0f, 0.0f, 0.0f)));

    allien->render(modelMatrix  * glm::translate(mat4(), vec3(10.0f, -2.0f, -170.0f))
                                * zRotationAnimation
                                * glm::rotate(mat4(), -1.570f, vec3(1.0f, 0.0f, 0.0f)));
}

void GLWidget::animate()
{
    position += velocity * time;
    mat4 view = viewMatrix * glm::translate(mat4(), position);

    if(gravity) {
       position.y -= position.y * time;
    }

    if(position.y >= 0) {
        position.y = 0;
    }

    glUseProgram(program);
    glUniformMatrix4fv(viewMatrixLoc, 1, false, value_ptr(view));

    float dt = .016;
    animationTime += dt;

    if(animationTime > 2*(rotations.size()-1)) {
        animationTime = 0;
    }

    float t = fmin(animationTime/(2*(rotations.size()-1)),1);

    unsigned int fromIndex = t*(rotations.size()-1);
    unsigned int toIndex = fromIndex+1;

    if(toIndex > rotations.size()-1) {
        toIndex = rotations.size()-1;
    }

    t = t*(rotations.size()-1)-(int)(t*(rotations.size()-1));

    vec3 from = rotations[fromIndex];
    vec3 to = rotations[toIndex];

    vec3 euler = (1 - t) * from + t * to;

    yRotationAnimation = rotate(mat4(), euler.y, vec3(0, 1, 0)) *
                         rotate(mat4(), euler.x, vec3(1, 0, 0));

    zRotationAnimation = rotate(mat4(), euler.z, vec3(0, 0, 1)) *
                         rotate(mat4(), euler.x, vec3(1, 0, 0));

    update();
}

void GLWidget::keyPressEvent(QKeyEvent *event) {
    switch(event->key()) {
        case Qt::Key_W:
            forward();
            break;
        case Qt::Key_A:
            left();
            break;
        case Qt::Key_S:
            backward();
            break;
        case Qt::Key_D:
            right();
            break;
        case Qt::Key_Shift:
            velocity.y = velocity.y + 10;
            break;
        case Qt::Key_Space:
            velocity.y = velocity.y - 1;
            break;
        case Qt::Key_Up:
            if(lightPosition.z > -1000) {
                lightPosition.z -= 10;
            }
            break;
        case Qt::Key_Down:
            if(lightPosition.z < 1000) {
                lightPosition.z += 10;
            }
            break;
        case Qt::Key_Right:
            if(lightPosition.x < 1000) {
                lightPosition.x += 10;
            }
            break;
        case Qt::Key_Left:
            if(lightPosition.x > -1000) {
                lightPosition.x -= 10;
            }
            break;
         case Qt::Key_Minus:
            if(lightIntensity > 0.5f) {
                lightIntensity -= 0.5;
            }
            break;
         case Qt::Key_Equal:
            if(lightIntensity < 2.0f) {
                lightIntensity += 0.5;
            }
            break;
          case Qt::Key_Q:
            specularEnable = !specularEnable;
            break;
          case Qt::Key_E:
            diffuseEnable = !diffuseEnable;
            break;
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event) {

    velocity = vec3(0);

    switch(event->key()) {
        case Qt::Key_G:
            gravity = !gravity;
            break;
    }
}

float GLWidget::toRadians(float angle) {
    return angle * M_PI/180;
}

void GLWidget::forward()
{
    velocity +=  forwardVector;
}

void GLWidget::backward()
{
    velocity -= forwardVector;
}

void GLWidget::left()
{
    velocity += rightVector;
}

void GLWidget::right()
{
    velocity -= rightVector;
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    vec2 pt(event->x(), event->y());
    lastPt = pt;
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    vec2 pt(event->x(), event->y());
    vec2 d = pt-lastPt;

    float sensibility = 1.0f;

    if(pt.x > 0 && pt.x < width && pt.y > 0 && pt.y < height) {
        pitch += (d.y) * sensibility;
        yaw += (d.x) * sensibility;

        if(pitch < -90.0f)
            pitch = -90.0f;

        if(pitch > 90.0f)
            pitch = 90.0f;

        if(yaw < -360.0f)
            yaw += 360.0f;

        if(yaw > 360.0f)
            yaw -= 360.0f;

        pitchMatrix = rotate(mat4(), toRadians(-pitch) , vec3(1, 0, 0));
        yawMatrix = rotate(mat4(), toRadians(-yaw), vec3(0, 1, 0));

        mat4 p = inverse(pitchMatrix);
        mat4 y = inverse(yawMatrix);
        viewMatrix =  p * y;

        forwardVector = vec3(yawMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
        rightVector = vec3(yawMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));

        glUseProgram(program);
        glUniformMatrix4fv(viewMatrixLoc, 1, false, value_ptr(viewMatrix));

        lastPt = pt;
    }
}


