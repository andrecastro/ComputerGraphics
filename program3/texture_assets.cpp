#include <texture_assets.h>

TextureAssets* TextureAssets::instance = NULL;

TextureAssets::TextureAssets()
{
    wall = new QOpenGLTexture(QImage(":/assets/wall.jpg"));
    wallBrick = new QOpenGLTexture(QImage(":/assets/wall2.jpeg"));
    roof = new QOpenGLTexture(QImage(":/assets/roof.jpg"));
    ground = new QOpenGLTexture(QImage(":/assets/ground.jpg"));
    grass = new QOpenGLTexture(QImage(":/assets/grass.jpg"));
    door = new QOpenGLTexture(QImage(":/assets/door2.jpg"));
    window = new QOpenGLTexture(QImage(":/assets/window.png"));
    pyramid = new QOpenGLTexture(QImage(":/assets/pyramid.jpg"));

    tree = new QOpenGLTexture(QImage(":models/tree/Tree.jpg"));
    barrel = new QOpenGLTexture(QImage(":/models/barrel/MedBarrelDiffuse.jpg"));
}

TextureAssets& TextureAssets::me()
{
    if(instance == NULL)
        instance = new TextureAssets();

    return *instance;
}





