#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>
#include <QPainter>
#include <QMenu>
#include <QAction>
#include <QFileDialog>

#include <iostream>
#include <math.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

using namespace std;
using namespace glm;

#define SQUARE      0
#define CIRCLE      1
#define TRIANGLE    2

struct color {
    float r;
    float g;
    float b;
};

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit GLWidget(QWidget *parent = 0);
    ~GLWidget();

public slots:
    void openNewImage();

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    void changeCurrentShape();
    void changeCurrentShapeSize(Qt::Key key);

    void fillScreenWithShape();
    void drawShape(vec2 centerPt);
    void drawCircle(float cx, float cy, float radius);
    void drawSquare(float cx, float cy, float size);
    void drawTriangle(float cx, float cy, float baseSize);

    color getImageColorAt(float x, float y);

    vec4 w2nd(float x, float y);
    vec4 imageCoord(vec4 ndcCoord);

    QOpenGLShaderProgram *program;
    QImage image;
    QImage scaledImage;

    int currentShape;
    float currentShapeSize;
    std::vector<vec2> pts; // vector to store the points
};

#endif
