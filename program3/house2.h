#ifndef HOUSE2
#define HOUSE2

#include <QOpenGLFunctions_3_3_Core>
#include "object_loader.h"

class House2 : ObjectLoader {

public:
    House2(GLuint &program);
    void render(mat4 matrix);

private:
    void initialize();

};

#endif // HOUSE2

