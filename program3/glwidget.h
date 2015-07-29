#ifndef __GLWIDGET__INCLUDE__
#define __GLWIDGET__INCLUDE__

#define GLM_FORCE_RADIANS

#include <QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMouseEvent>
#include <QTimer>
#include <glm/glm.hpp>

#include "cube.h"
#include "skybox.h"
#include "ground.h"
#include "house.h"
#include "pyramid.h"
#include "tree.h"
#include "grass.h"
#include "barrel.h"
#include "well.h"
#include "allien.h"

#include "shader_loader.h"

using namespace glm;

class GLWidget : public QOpenGLWidget, protected ShaderLoader {
    Q_OBJECT

public:
    GLWidget(QWidget *parent=0);
    ~GLWidget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

protected slots:
    void animate();

private:
    float toRadians(float angle);

    void forward();
    void backward();
    void left();
    void right();

    GLuint program;
    GLint projMatrixLoc;
    GLint viewMatrixLoc;
    GLint modelMatrixLoc;

    mat4 projMatrix;
    mat4 viewMatrix;
    mat4 modelMatrix;

    mat4 pitchMatrix;
    mat4 yawMatrix;

    vec3 forwardVector;
    vec3 rightVector;

    float pitch;
    float yaw;

    QTimer *timer;

    bool gravity;
    bool specularEnable;
    bool diffuseEnable;

    vec3 position;
    vec3 velocity;

    float distance;
    float time;
    float lightIntensity;

    float animationTime;

    int width;
    int height;

    vec2 lastPt;
    vec3 lightPosition;

    void updateView();

    Cube *cube;
    Skybox *skybox;
    Ground *ground;
    House *house;
    Pyramid *pyramid;
    Tree *tree;
    Grass *grass;
    Well *well;
    Barrel *barrel;
    Allien *allien;

    std::vector<vec3> rotations;
    mat4 yRotationAnimation;
    mat4 zRotationAnimation;
};

#endif
