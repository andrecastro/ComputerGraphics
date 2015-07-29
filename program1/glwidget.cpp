#include "glwidget.h"

#define PROGRAM_VERTEX_ATTRIBUTE 0

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      program(0)
{
    currentShape = SQUARE;
    currentShapeSize = 0.05;
}

GLWidget::~GLWidget()
{
    makeCurrent();
    delete program;
    doneCurrent();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    image = QImage(QString(":/images/lake.png"));
    int ratio = devicePixelRatio();
    resize(image.width()/ratio, image.height()/ratio);
}

// scale algorithm based on the algorithm provided at
// http://stackoverflow.com/questions/14107144/how-do-i-make-an-image-resize-to-scale-in-qt
void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QSize imgSize = image.size();
    imgSize.scale(this->rect().size(), Qt::KeepAspectRatio);

    scaledImage = image.scaled(imgSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPoint point = this->rect().bottomRight() - scaledImage.rect().bottomRight();

    painter.drawImage(point/2, scaledImage);
    painter.end();

    for (unsigned long var = 0; var < pts.size(); var++) {
        drawShape(pts[var]);
    }
}

void GLWidget::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{    
    if(event->button() == Qt::RightButton)
    {
        QMenu menu;
        QAction* openAct = new QAction("Open...", this);
        connect(openAct, SIGNAL(triggered()), this, SLOT(openNewImage()));
        menu.addAction(openAct);
        menu.addSeparator();
        menu.exec(mapToGlobal(event->pos()));
    } else {
        pts.push_back(vec2(event->x(), event->y()));
        update();
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
     pts.push_back(vec2(event->x(), event->y()));
     update();
}

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_C: // clear all points
            pts.clear();
            break;
        case Qt::Key_S: // change the current shape
            changeCurrentShape();
            break;
        case Qt::Key_Up: // change up the shape size
            changeCurrentShapeSize(Qt::Key_Up);
            break;
        case Qt::Key_Down: // change down the shape size
            changeCurrentShapeSize(Qt::Key_Down);
            break;
        case Qt::Key_F: // fill all the window with current shape
            fillScreenWithShape();
            break;
        default:
            break;
    }
    update();
}

// changes the current shape
// CIRCLE, TRIANGLE, SQUARE
void GLWidget::changeCurrentShape()
{
    switch (currentShape)
    {
        case CIRCLE:
            currentShape = SQUARE;
            break;
        case SQUARE:
            currentShape = TRIANGLE;
            break;
        case TRIANGLE:
            currentShape = CIRCLE;
            break;
    }
}

// changes the size of the shape
void GLWidget::changeCurrentShapeSize(Qt::Key key)
{
    switch (key)
    {
        case Qt::Key_Up:
            currentShapeSize += 0.01;
            break;
        case Qt::Key_Down:
            currentShapeSize -= 0.01;
            break;
        default:
            break;
    }

    if(currentShapeSize > 0.05) {
        currentShapeSize = 0.01;
    } else if(currentShapeSize < 0) {
        currentShapeSize = 0.05;
    }
}

// fills the screen with the shape pixel
void GLWidget::fillScreenWithShape()
{
    pts.clear();
    float base = currentShapeSize * 100;
    vec2 baseOver2 = vec2(base/2, base/2);

    for(float y = 0; y < this->height(); y += base) {
        for(float x = 0; x < this->width(); x += base) {
            pts.push_back(vec2(x, y) + baseOver2);
        }
    }
}

// draws a shape based on the current shape
void GLWidget::drawShape(vec2 point)
{
    vec4 centerPt = w2nd(point.x, point.y);
    switch (currentShape)
    {
        case CIRCLE:
            drawCircle(centerPt.x, centerPt.y, currentShapeSize);
            break;
        case SQUARE:
            drawSquare(centerPt.x, centerPt.y, currentShapeSize);
            break;
        case TRIANGLE:
            drawTriangle(centerPt.x, centerPt.y, currentShapeSize);
            break;
    }
}

// draws a circle
// algorithm based on the algorithm povided at
// http://stackoverflow.com/questions/22444450/drawing-circle-with-opengl
void GLWidget::drawCircle(float cx, float cy, float radius)
{
    glBegin(GL_POLYGON);
    for (int i=0; i < 20; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(20);
        float newX = cx + radius * cosf(theta);
        float newY = cy + radius * sinf(theta);

        vec4 imgCoord = imageCoord(vec4(newX, newY, 0, 1));
        color c = getImageColorAt(imgCoord.x, imgCoord.y);
        glColor3f(c.r, c.g, c.b);
        glVertex2f(newX, newY);
    }
    glEnd();
}

// draws a square
void GLWidget::drawSquare(float cx, float cy, float size)
{
    float invariant = size/2;
    vec4 A(cx + invariant, cy - invariant, 0, 1);
    vec4 imgCoord = imageCoord(vec4(A.x, A.y, 0, 1));
    color cA = getImageColorAt(imgCoord.x, imgCoord.y);

    vec4 B(cx - invariant, cy - invariant, 0, 1);
    imgCoord = imageCoord(vec4(B.x, B.y, 0, 1));
    color cB = getImageColorAt(imgCoord.x, imgCoord.y);

    vec4 C(cx - invariant, cy + invariant, 0, 1);
    imgCoord = imageCoord(vec4(C.x, C.y, 0, 1));
    color cC = getImageColorAt(imgCoord.x, imgCoord.y);

    vec4 D(cx + invariant, cy + invariant, 0, 1);
    imgCoord = imageCoord(vec4(D.x, D.y, 0, 1));
    color cD = getImageColorAt(imgCoord.x, imgCoord.y);

    glBegin(GL_POLYGON);
        glColor3f(cA.r, cA.g, cA.b); glVertex2f(A.x, A.y);
        glColor3f(cB.r, cB.g, cB.b); glVertex2f(B.x, B.y);
        glColor3f(cC.r, cC.g, cC.b); glVertex2f(C.x, C.y);
        glColor3f(cD.r, cD.g, cD.b); glVertex2f(D.x, D.y);
    glEnd();
}

// draws a triangle
void GLWidget::drawTriangle(float cx, float cy, float baseSize)
{
    float invariant = baseSize/2;
    vec4 A(cx + invariant, cy - invariant, 0, 1);
    vec4 imgCoord = imageCoord(vec4(A.x, A.y, 0, 1));
    color cA = getImageColorAt(imgCoord.x, imgCoord.y);

    vec4 B(cx - invariant, cy - invariant, 0, 1);
    imgCoord = imageCoord(vec4(B.x, B.y, 0, 1));
    color cB = getImageColorAt(imgCoord.x, imgCoord.y);

    vec4 C(cx, cy + invariant, 0, 1);
    imgCoord = imageCoord(vec4(C.x, C.y, 0, 1));
    color cC = getImageColorAt(imgCoord.x, imgCoord.y);

    glBegin(GL_POLYGON);
        glColor3f(cA.r, cA.g, cA.b); glVertex2f(A.x, A.y);
        glColor3f(cB.r, cB.g, cB.b); glVertex2f(B.x, B.y);
        glColor3f(cC.r, cC.g, cC.b); glVertex2f(C.x, C.y);
    glEnd();
}

// gets the color of the image at the position(x,y)
// if (x,y) is out of range returns a color white
color GLWidget::getImageColorAt(float x, float y)
{
    color c;
    if(scaledImage.valid(x, y)) {
        QColor pixelColor(scaledImage.pixel(x, y));
        c.r = static_cast<float>(pixelColor.red())/255.0;
        c.g = static_cast<float>(pixelColor.green())/255.0;
        c.b = static_cast<float>(pixelColor.blue())/255.0;
    } else {
        c.r = 0;
        c.g = 0;
        c.b = 0;
    }
    return c;
}

// transforms word coordinates to normalized device coordinates
vec4 GLWidget::w2nd(float x, float y)
{
    float ndx = (x * (2.0/width())) - 1.0;
    float ndy = 1.0 - (y * (2.0/height()));
    return vec4(ndx, ndy, 0, 1);
}

// transforms the normalized device coordinates to image coordinates
// algorithm based on the algorithm provided by @jallwine at
// https://github.com/jallwine/MSU-CSCI441-Spring-2015/blob/master/examples/resizing.cpp
vec4 GLWidget::imageCoord(vec4 ndcCoord)
{
    float width = (float) this->width();
    float height = (float) this->height();
    float invaspect = height/width;

    mat4 ws2ndc = ortho(-1.0f, 1.0f, -invaspect, invaspect);
    mat4 ndc2ws = inverse(ws2ndc);
    vec4 wsCoord = ndc2ws * ndcCoord;
    mat4 ws2img(.5 * scaledImage.width(), 0, 0, 0,
                    0, -.5 * scaledImage.width(), 0, 0,
                    0, 0, 1, 0,
                    .5 * scaledImage.width(), .5 * scaledImage.height(), 0, 1);

    vec4 imgCoord = ws2img * wsCoord;
    return imgCoord;
}

// opens a new image
void GLWidget::openNewImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "images", tr("Image Files (*.png *.jpg *.bmp)"));

    if(!fileName.isNull()) {
        image.load(fileName);
        int ratio = devicePixelRatio();
        resize(image.width()/ratio, image.height()/ratio);
        update();
    }
    activateWindow();
}
