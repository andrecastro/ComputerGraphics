#ifndef __GLWIDGET__INCLUDE__
#define __GLWIDGET__INCLUDE__

#include "shader_loader.h"
#include <QOpenGLTexture>
#include <QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QMouseEvent>
#include <glm/glm.hpp>

#define GLM_FORCE_RADIANS

using namespace glm;

class GLWidget :  public QOpenGLWidget, protected ShaderLoader {
    Q_OBJECT

    public:
        GLWidget(QWidget *parent=0);
        ~GLWidget();

    public slots:
        void changeWallSize(int newSize);
        void changeBrickSpace(int newSpace);

        void changeBrickWidth(int newWidth);
        void changeBrickHeight(int newWidth);
        void changeBrickDepth(int newWidth);

        void toggleDiffuse(bool diffuse);
        void toggleSpecular(bool specular);
        void toggleTexture(bool texture);

        void toggleType(bool type);
        void changeSpecularShininess(int shininess);

    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();

        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent *event);

    private:
        void initializeCube();
        void renderCube(mat4 matrix);
        void renderWall(mat4 matrix);
        void renderBrick(mat4 baseMatrix);
        void renderSquareWall();


        void initializeGrid();
        void renderGrid();

        void bindTextureImage(QImage image);
        void bindColor(vec3 color);
        void bindColor(vec3 top, vec3 bottom, vec3 right, vec3 left, vec3 front, vec3 back);

        GLuint wallProg;
        GLuint brickVao;
        GLuint wallVao;
        GLint wallProjecMatrixLoc;
        GLint wallViewMatrixLoc;
        GLint wallModelMatrixLoc;

        GLint enableDiffuseLoc;
        GLint enableSpecularLoc;
        GLint enableTextureLoc;
        GLint specularShininessLoc;

        GLuint gridProg;
        GLuint gridVao;
        GLint gridProjMatrixLoc;
        GLint gridViewMatrixLoc;
        GLint gridModelMatrixLoc;

        mat4 projMatrix;
        mat4 viewMatrix;
        mat4 wallModelMatrix;
        mat4 gridModelMatrix;

        GLuint colorBuffer;
        QOpenGLTexture *tex;

        int width;
        int height;

        float viewDist;

        vec3 lastVPt;
        vec3 pointOnVirtualTrackball(const vec2 &pt); 

        bool rowType;

        float brickWidth;
        float brickHeight;
        float brickDepth;
        float space;

        float wallSize;
        float wallHeight;
        float wallWidth;
        float wallDepth;

        float constant;
};

#endif
