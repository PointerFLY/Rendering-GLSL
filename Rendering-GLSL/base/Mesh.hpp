//
//  Mesh.hpp
//  Rendering-GLSL
//
//  Created by PointerFLY on 05/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Mesh {
public:
    ~Mesh();
    Mesh(const std::string& fileName);
    Mesh(const std::vector<glm::vec3>& positions,
         const std::vector<glm::vec3>& normals = {},
         const std::vector<glm::vec2>& textureCoords = {},
         const std::vector<GLuint>& indices = {});
    
    virtual void init(GLuint programID);
    virtual void draw();
    
protected:
    std::vector<glm::vec3> _positions;
    std::vector<glm::vec2> _textureCoords;
    std::vector<glm::vec3> _normals;
    std::vector<GLuint> _indices;
    GLuint _vao;
    enum VBOIndex {
        POSITION_VB = 0,
        TEXTURE_VB,
        NORMAL_VB,
        INDEX_VB,
    };
    static const unsigned int NUM_BUFFERS = 4;
    GLuint _vbos[NUM_BUFFERS];
};

#endif /* Mesh_hpp */
