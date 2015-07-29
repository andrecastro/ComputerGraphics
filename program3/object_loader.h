#ifndef OBJECT_LOADER
#define OBJECT_LOADER

#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <tinyobjloader/tiny_obj_loader.h>
#include <QFile>
#include <iostream>

using namespace glm;
using namespace std;

class ObjectLoader {

public:
    ObjectLoader(GLuint &program);

    void initializeModel(const char* filename, bool smooth = false);
    void renderObject(mat4 matrix);


private:
    void initializeFlatModel(vector<tinyobj::shape_t> shapes,
                             vector<vec3> &positions,
                             vector<vec3> &normals,
                             vector<vec2> &texture);

    void initializeSmoothModel(vector<tinyobj::shape_t> shapes,
                               vector<vec3> &positions,
                               vector<vec3> &normals,
                               vector<vec2> &texture,
                               vector<unsigned int> &indices);

    void uploadData();
    void renderModelFlat();
    void renderModelSmooth();

    GLuint modelVao;
    GLint modelMatrixLoc;

    int numFlatVertices;
    int numSmoothIndices;

    bool smooth;

    GLuint positionBuffer;
    GLuint normalBuffer;
    GLuint texBuffer;
    GLuint smoothIndexBuffer;

    vector<vec3> positions;
    vector<vec3> normals;
    vector<vec2> texture_coord;
    vector<unsigned int> smooth_indices;

    GLuint program;
};

#endif // OBJECT_LOADER

