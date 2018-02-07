//
//  Mesh.cpp
//  Rendering-GLSL
//
//  Created by PointerFLY on 05/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#include "Mesh.hpp"
#include <iostream>

Mesh::Mesh(const std::string& fileName) {
    // TODO: Create from files, expecially add *.obj file support.
    std::cerr << "Create mesh from file is not supported now!" << std::endl;
    exit(EXIT_FAILURE);
}

Mesh::Mesh(const std::vector<glm::vec3>& positions,
           const std::vector<glm::vec3>& normals,
           const std::vector<glm::vec2>& textureCoords,
           const std::vector<GLint>& indices) {
    _positions = positions;
    _normals = normals;
    _textureCoords = textureCoords;
}

void Mesh::init(GLuint programID) {
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    
    glGenBuffers(NUM_BUFFERS, _vbos);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_positions.at(0)) * _positions.size(), _positions.data(), GL_STATIC_DRAW);
    GLuint vPosition = glGetAttribLocation(programID, "position");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_normals.at(0)) * _normals.size(), _normals.data(), GL_STATIC_DRAW);
    GLint vNormal = glGetAttribLocation(programID, "normal");
    glEnableVertexAttribArray(vNormal);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[TEXTURE_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_textureCoords.at(0)) * _textureCoords.size(), _textureCoords.data(), GL_STATIC_DRAW);
    GLint vTextureCoord = glGetAttribLocation(programID, "textureCoord");
    glEnableVertexAttribArray(vTextureCoord);
    glVertexAttribPointer(vTextureCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    if (!_indices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbos[INDEX_VB]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices.at(0)) * _indices.size(), _indices.data(), GL_STATIC_DRAW);
    }
}

Mesh::~Mesh() {
    glDeleteBuffers(NUM_BUFFERS, _vbos);
    glDeleteVertexArrays(1, &_vao);
}

void Mesh::draw() {
    glBindVertexArray(_vao);
    if (_indices.empty()) {
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(_positions.size()));
    } else {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_indices.size()), GL_UNSIGNED_INT, 0);
    }
}



