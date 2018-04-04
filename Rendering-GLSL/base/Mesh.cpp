//
//  Mesh.cpp
//  Rendering-GLSL
//
//  Created by PointerFLY on 05/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#include "Mesh.hpp"
#include <iostream>

Mesh* processMesh(aiMesh *aiMesh, const aiScene *scene) {
    //    std::vector<glm::vec3> _positions;
    //    std::vector<glm::vec2> _textureCoords;
    //    std::vector<glm::vec3> _normals;
    //    std::vector<GLuint> _indices;
    Mesh* mesh = new Mesh(std::vector<glm::vec3>());
    
    for(int i = 0; i < aiMesh->mNumVertices; i++) {
        aiVector3t<float> aiPosition = aiMesh->mVertices[i];
        glm::vec3 position = glm::vec3(aiPosition.x, aiPosition.y, aiPosition.z);
        mesh->_positions.push_back(position);
        
        aiVector3t<float> aiNormal = aiMesh->mNormals[i];
        glm::vec3 normal = glm::vec3(aiNormal.x, aiNormal.y, aiNormal.z);
        mesh->_normals.push_back(normal);
        
        if (aiMesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = aiMesh->mTextureCoords[0][i].x;
            vec.y = aiMesh->mTextureCoords[0][i].y;
            mesh->_textureCoords.push_back(vec);
        }
    }
    
    for(unsigned int i = 0; i < aiMesh->mNumFaces; i++) {
        aiFace face = aiMesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
        mesh->_indices.push_back(face.mIndices[j]);
    }
    
    return mesh;
}

std::vector<Mesh*> meshes;

void processNode(aiNode *node, const aiScene *scene) {
    // process each mesh located at the current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene.
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
        Mesh* mesh = processMesh(aiMesh, scene);
        mesh->afterContructor();
        meshes.push_back(mesh);
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

std::vector<Mesh*> Mesh::createMeshes(const std::string & fileName) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        exit(EXIT_FAILURE);
    }
    
    processNode(scene->mRootNode, scene);
    return meshes;
}

Mesh::Mesh(const std::string& fileName) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        exit(EXIT_FAILURE);
    }
    
    processNode(scene->mRootNode, scene);
    
    afterContructor();
}

void Mesh::afterContructor() {
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glGenBuffers(NUM_BUFFERS, _vbos);
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbos[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_positions.at(0)) * _positions.size(), _positions.data(), GL_STATIC_DRAW);
    
    if (!_normals.empty()) {
        assert(_normals.size() == _positions.size());
        glBindBuffer(GL_ARRAY_BUFFER, _vbos[NORMAL_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_normals.at(0)) * _normals.size(), _normals.data(), GL_STATIC_DRAW);
    }
    
    if (!_textureCoords.empty()) {
        assert(_textureCoords.size() == _positions.size());
        glBindBuffer(GL_ARRAY_BUFFER, _vbos[TEXTURE_VB]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_textureCoords.at(0)) * _textureCoords.size(), _textureCoords.data(), GL_STATIC_DRAW);
    }
    
    if (!_indices.empty()) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbos[INDEX_VB]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices.at(0)) * _indices.size(), _indices.data(), GL_STATIC_DRAW);
    }
}

Mesh::Mesh(const std::vector<glm::vec3>& positions,
           const std::vector<glm::vec3>& normals,
           const std::vector<glm::vec2>& textureCoords,
           const std::vector<GLuint>& indices) {
//    assert(positions.size() > 0);
    
    _positions = positions;
    _normals = normals;
    _textureCoords = textureCoords;
    _indices = indices;

//    afterContructor();
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



