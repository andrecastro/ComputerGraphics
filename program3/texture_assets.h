#ifndef TEXTURE_ASSETS
#define TEXTURE_ASSETS

#include <QOpenGLTexture>

class TextureAssets {

public:
    static TextureAssets& me();

    QOpenGLTexture *wall;
    QOpenGLTexture *wallBrick;
    QOpenGLTexture *roof;
    QOpenGLTexture *ground;
    QOpenGLTexture *grass;
    QOpenGLTexture *door;
    QOpenGLTexture *window;
    QOpenGLTexture *pyramid;

    QOpenGLTexture *tree;
    QOpenGLTexture *barrel;

    QOpenGLTexture *skyboxSide1;
    QOpenGLTexture *skyboxSide2;
    QOpenGLTexture *skyboxSide3;
    QOpenGLTexture *skyboxSide4;
    QOpenGLTexture *skyboxSide5;

private:
    TextureAssets();
    static TextureAssets *instance;
};

#endif // ASSETS

