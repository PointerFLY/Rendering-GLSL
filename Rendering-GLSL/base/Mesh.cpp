//
//  Mesh.cpp
//  Rendering-GLSL
//
//  Created by PointerFLY on 05/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#include "Mesh.hpp"
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::shared_ptr<Mesh> processMesh(aiMesh *aiMesh, const aiScene *scene) {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textureCoords;
    std::vector<GLuint> indices;
    
    for (int i = 0; i < aiMesh->mNumVertices; i++) {
        aiVector3t<float> aiPosition = aiMesh->mVertices[i];
        glm::vec3 position = glm::vec3(aiPosition.x, aiPosition.y, aiPosition.z);
        positions.push_back(position);
        
        aiVector3t<float> aiNormal = aiMesh->mNormals[i];
        glm::vec3 normal = glm::vec3(aiNormal.x, aiNormal.y, aiNormal.z);
        normals.push_back(normal);
    
        if (aiMesh->mTextureCoords[0]) {
            glm::vec2 textureCoord;
            textureCoord.x = aiMesh->mTextureCoords[0][i].x;
            textureCoord.y = aiMesh->mTextureCoords[0][i].y;
            textureCoords.push_back(textureCoord);
        }
    }
    
    for (int i = 0; i < aiMesh->mNumFaces; i++) {
        aiFace face = aiMesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    return std::make_shared<Mesh>(positions, normals, textureCoords, indices);
}

void processNode(aiNode *node, const aiScene *scene, std::vector<std::shared_ptr<Mesh>>& meshes) {
    for (int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
        std::shared_ptr<Mesh> mesh = processMesh(aiMesh, scene);
        meshes.push_back(mesh);
    }

    for (int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene, meshes);
    }
}

std::vector<std::shared_ptr<Mesh>> Mesh::createFromFile(const std::string& fileName) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::vector<std::shared_ptr<Mesh>> meshes;
    processNode(scene->mRootNode, scene, meshes);
    return meshes;
}

Mesh::Mesh(const std::vector<glm::vec3>& positions,
           const std::vector<glm::vec3>& normals,
           const std::vector<glm::vec2>& textureCoords,
           const std::vector<GLuint>& indices) {
    _positions = positions;
    _normals = normals;
    _textureCoords = textureCoords;
    _indices = indices;
    
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glGenBuffers(NUM_BUFFERS, _vbos);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_positions.at(0)) * _positions.size(), _positions.data(), GL_STATIC_DRAW);
    
    if (!normals.empty()) {
        assert(normals.size() == positions.size());
        glBindBuffer(GL_ARRAY_BUFFER, _vbos[NORMAL_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_normals.at(0)) * _normals.size(), _normals.data(), GL_STATIC_DRAW);
    }
    
    if (!textureCoords.empty()) {
        assert(textureCoords.size() == positions.size());
        glBindBuffer(GL_ARRAY_BUFFER, _vbos[TEXTURE_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_textureCoords.at(0)) * _textureCoords.size(), _textureCoords.data(), GL_STATIC_DRAW);
    }
    
    if (!_indices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbos[INDEX_VB]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices.at(0)) * _indices.size(), _indices.data(), GL_STATIC_DRAW);
    }
}

void Mesh::init(GLuint programID) {
    glBindVertexArray(_vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[POSITION_VB]);
    GLuint vPosition = glGetAttribLocation(programID, "position");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    if (!_normals.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, _vbos[NORMAL_VB]);
        GLint vNormal = glGetAttribLocation(programID, "normal");
        glEnableVertexAttribArray(vNormal);
        glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }
    
    if (!_textureCoords.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, _vbos[TEXTURE_VB]);
        GLint vTextureCoord = glGetAttribLocation(programID, "textureCoord");
        glEnableVertexAttribArray(vTextureCoord);
        glVertexAttribPointer(vTextureCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);
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



