#ifndef SHADER_LOADER
#define SHADER_LOADER

#include <iostream>
#include <QFile>
#include <QOpenGLFunctions_3_3_Core>
#include <QTextStream>


class ShaderLoader: protected QOpenGLFunctions_3_3_Core {

public:
    ShaderLoader();
    GLuint loadShaders(const char* vertf, const char* fragf);
};

#endif // SHADER_LOADER

