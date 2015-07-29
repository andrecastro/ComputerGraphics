#include "object_loader.h"

ObjectLoader::ObjectLoader(GLuint &program)
{
    this->program = program;
}

void ObjectLoader::initializeModel(const char *filename,  bool smooth)
{
    this->smooth = smooth;

    vector<tinyobj::shape_t> shapes;
    vector<tinyobj::material_t> materials;

    string err = tinyobj::LoadObj(shapes, materials, filename);

    if(!err.empty()) {
        cerr << err << endl;
        exit(1);
    }

    cout << "# of shape: " << shapes[0].name << endl;
    cout << "# of shapes: " << shapes.size() << endl;
    cout << "# of materials: " << materials.size() << endl;

    if(!smooth) {
        initializeFlatModel(shapes, positions, normals, texture_coord);
        numFlatVertices = normals.size();
    } else {
        initializeSmoothModel(shapes, positions, normals, texture_coord, smooth_indices);
        numSmoothIndices = smooth_indices.size();
    }

     // Upload all our data to buffers
     glGenBuffers(1, &positionBuffer);
     glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
     glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

     glGenBuffers(1, &normalBuffer);
     glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
     glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

     glGenBuffers(1, &texBuffer);
     glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
     glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

     glUseProgram(program);

     // get some variable positions
     GLint positionIndex = glGetAttribLocation(program, "position");
     GLint normalIndex = glGetAttribLocation(program, "normal");
     GLint textureIndex = glGetAttribLocation(program, "uv");

     // bind our buffers to vertex array objects and shader attributes
     glGenVertexArrays(1, &modelVao);
     glBindVertexArray(modelVao);

     glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
     glEnableVertexAttribArray(positionIndex);
     glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

     glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
     glEnableVertexAttribArray(normalIndex);
     glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

     glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
     glEnableVertexAttribArray(textureIndex);
     glVertexAttribPointer(textureIndex, 2, GL_FLOAT, GL_FALSE, 0, 0);


     if(smooth) {
         glGenBuffers(1, &smoothIndexBuffer);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, smoothIndexBuffer);
         glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);

         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, smoothIndexBuffer);
     }

    uploadData();
    modelMatrixLoc = glGetUniformLocation(program, "model");
}

void ObjectLoader::uploadData()
{

    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, positions.size()*sizeof(vec3), &positions[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(vec3), &normals[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, texBuffer);
    glBufferData(GL_ARRAY_BUFFER, texture_coord.size()*sizeof(vec2), &texture_coord[0], GL_DYNAMIC_DRAW);

    if(smooth) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, smoothIndexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, smooth_indices.size()*sizeof(unsigned int), &smooth_indices[0], GL_DYNAMIC_DRAW);
    }
}

void ObjectLoader::initializeFlatModel(vector<tinyobj::shape_t> shapes, vector<vec3> &positions, vector<vec3> &normals, vector<vec2> &texture)
{
    positions.clear();
    normals.clear();

    for(size_t i = 0; i < shapes.size(); i++) {
        tinyobj::shape_t shape = shapes[i];

        for(size_t tri = 0; tri < shape.mesh.indices.size() / 3; tri++) {
            // Here we're getting the indices for the current triangle
            unsigned int ind0 = shape.mesh.indices[3*tri+0];
            unsigned int ind1 = shape.mesh.indices[3*tri+1];
            unsigned int ind2 = shape.mesh.indices[3*tri+2];

            // Using those indices we can get the three points of the triangle
            vec3 p0 = vec3(shape.mesh.positions[3*ind0+0],
                           shape.mesh.positions[3*ind0+1],
                           shape.mesh.positions[3*ind0+2]);

            vec3 p1 = vec3(shape.mesh.positions[3*ind1+0],
                           shape.mesh.positions[3*ind1+1],
                           shape.mesh.positions[3*ind1+2]);

            vec3 p2 = vec3(shape.mesh.positions[3*ind2+0],
                           shape.mesh.positions[3*ind2+1],
                           shape.mesh.positions[3*ind2+2]);

            vec3 p0p1 = p1 - p0;
            vec3 p0p2 = p2 - p0;

            vec3 n = normalize(cross(p0p1, p0p2));

            positions.push_back(p0);
            positions.push_back(p1);
            positions.push_back(p2);

            normals.push_back(n);
            normals.push_back(n);
            normals.push_back(n);
        }

        for(size_t t = 0; t < shape.mesh.texcoords.size(); t+= 2) {
            texture.push_back(vec2(shape.mesh.texcoords[t], shape.mesh.texcoords[t + 1]));
        }
    }

    std::cout << positions.size() << endl;
    std::cout << texture.size() << endl;
}

void ObjectLoader::initializeSmoothModel(vector<tinyobj::shape_t> shapes, vector<vec3> &positions, vector<vec3> &normals, vector<vec2> &texture, vector<unsigned int> &indices)
{
    positions.clear();
    normals.clear();

    for(size_t i = 0; i < shapes.size(); i++) {
        tinyobj::shape_t shape = shapes[i];

        for(size_t tri = 0; tri < shape.mesh.positions.size(); tri+= 3) {
            vec3 p(shape.mesh.positions[tri], shape.mesh.positions[tri + 1], shape.mesh.positions[tri + 2]);
            positions.push_back(p);
        }

        for(size_t t = 0; t < shape.mesh.texcoords.size(); t+= 2) {
            texture.push_back(vec2(shape.mesh.texcoords[t], shape.mesh.texcoords[t + 1]));
        }

        for(size_t i = 0; i < positions.size(); i++) {
            normals.push_back(vec3(0, 0, 0));
        }

        for(size_t tri = 0; tri < shape.mesh.indices.size() / 3; tri++) {
            // Here we're getting the indices for the current triangle
            unsigned int ind0 = shape.mesh.indices[3*tri+0];
            unsigned int ind1 = shape.mesh.indices[3*tri+1];
            unsigned int ind2 = shape.mesh.indices[3*tri+2];

            // Using those indices we can get the three points of the triangle
            vec3 p0 = vec3(shape.mesh.positions[3*ind0+0],
                           shape.mesh.positions[3*ind0+1],
                           shape.mesh.positions[3*ind0+2]);

            vec3 p1 = vec3(shape.mesh.positions[3*ind1+0],
                           shape.mesh.positions[3*ind1+1],
                           shape.mesh.positions[3*ind1+2]);

            vec3 p2 = vec3(shape.mesh.positions[3*ind2+0],
                           shape.mesh.positions[3*ind2+1],
                           shape.mesh.positions[3*ind2+2]);

            vec3 p0p1 = p1 - p0;
            vec3 p0p2 = p2 - p0;

            vec3 n = normalize(cross(p0p1, p0p2));

            normals[ind0] += n;
            normals[ind1] += n;
            normals[ind2] += n;
        }

        for(size_t i = 0; i < shape.mesh.indices.size(); i++) {
            indices.push_back(shape.mesh.indices[i]);
        }

        for(size_t i = 0; i < normals.size(); i++) {
            normals[i] = normalize(normals[i]);
        }
    }

    std::cout << positions.size() << endl;
    std::cout << texture.size() << endl;
}

void ObjectLoader::renderObject(mat4 matrix)
{

    glUseProgram(program);
    glUniformMatrix4fv(modelMatrixLoc, 1, false, value_ptr(matrix));

    if(smooth) {
        renderModelSmooth();
    } else {
        renderModelFlat();
    }
}

void ObjectLoader::renderModelFlat()
{
    glBindVertexArray(modelVao);
    glDrawArrays(GL_TRIANGLES, 0, numFlatVertices);
}

void ObjectLoader::renderModelSmooth()
{
    glBindVertexArray(modelVao);
    glDrawElements(GL_TRIANGLES, numSmoothIndices, GL_UNSIGNED_INT, 0);
}
