//
//  main.cpp
//  Lab2
//
//  Created by PointerFLY on 28/01/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "GLApplication.hpp"
#include "GLProgram.hpp"
#include "teapot.h"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Skybox.hpp"
#include "CubeMap.hpp"

static std::unique_ptr<GLApplication> app;
static std::unique_ptr<GLProgram> skyboxProgram;
static std::unique_ptr<GLProgram> meshProgram;
static std::unique_ptr<Mesh> skybox;
static std::unique_ptr<Mesh> mesh;
static std::unique_ptr<CubeMap> cubeMap;

void update() {
    float width = app->getWindowSize().getWidth();
    float height = app->getWindowSize().getHeight();
    
    glDepthMask(GL_FALSE);
    skybox->init(skyboxProgram->getID());
    skyboxProgram->use();
    skyboxProgram->setDefaultMats();
    cubeMap->bind();
    skybox->draw();
    glDepthMask(GL_TRUE);
    
    mesh->init(meshProgram->getID());
    meshProgram->use();
    glm::mat4 modelMat;
    glm::mat4 viewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 60.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projMat = glm::perspective(glm::radians(75.0f), width / height, 0.1f, 60.0f);
    meshProgram->setMat(modelMat, GLProgram::MatType::MODEL);
    meshProgram->setMat(viewMat, GLProgram::MatType::VIEW);
    meshProgram->setMat(projMat, GLProgram::MatType::PROJ);
    mesh->draw();
}

int main() {
    app = std::make_unique<GLApplication>("Reflectance Models", 1200, 750);
    
    // Skybox
    
    skyboxProgram = std::make_unique<GLProgram>();
    skyboxProgram->create();
    skyboxProgram->addShader("shaders/skybox_vs.glsl", GLProgram::ShaderType::VERTEXT);
    skyboxProgram->addShader("shaders/skybox_fs.glsl", GLProgram::ShaderType::FRAGMENT);
    skyboxProgram->link();

    skybox = std::make_unique<Skybox>(100.0f);
    
    std::vector<std::string> fileNames = {
        "assets/skyboxes/lake/right.jpg",
        "assets/skyboxes/lake/left.jpg",
        "assets/skyboxes/lake/top.jpg",
        "assets/skyboxes/lake/bottom.jpg",
        "assets/skyboxes/lake/back.jpg",
        "assets/skyboxes/lake/front.jpg",
    };
    cubeMap = std::make_unique<CubeMap>(fileNames);
    
    // Mesh
    
    meshProgram = std::make_unique<GLProgram>();
    meshProgram->create();
    meshProgram->addShader("shaders/vs.glsl", GLProgram::ShaderType::VERTEXT);
    meshProgram->addShader("shaders/fs.glsl", GLProgram::ShaderType::FRAGMENT);
    meshProgram->link();
    
    static_assert(sizeof(glm::vec3) == sizeof(float) * 3, "sizeof(glm::vec3) != sizeof(float) * 3");
    glm::vec3* positionArray = reinterpret_cast<glm::vec3*>(teapot_vertex_points);
    glm::vec3* normalArray = reinterpret_cast<glm::vec3*>(teapot_normals);
    glm::vec2* textureCoordsArray = reinterpret_cast<glm::vec2*>(teapot_tex_coords);
    std::vector<glm::vec3> positions(positionArray, positionArray + static_cast<size_t>(teapot_vertex_count));
    std::vector<glm::vec3> normals(normalArray, normalArray + static_cast<size_t>(teapot_vertex_count));
    std::vector<glm::vec2> textureCoords(textureCoordsArray, textureCoordsArray + static_cast<size_t>(teapot_vertex_count));
    std::vector<GLuint> indices;
    mesh = std::make_unique<Mesh>(positions, normals, textureCoords, indices);
    
    app->mainLoop(update);
    
    return 0;
}
