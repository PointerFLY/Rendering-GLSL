//
//  main.cpp
//  surface-mapping
//
//  Created by PointerFLY on 08/02/2018.
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
static std::unique_ptr<Mesh> skybox;
static std::unique_ptr<Mesh> brick;
static std::unique_ptr<CubeMap> cubeMap;
static std::unique_ptr<Texture> texture;
static std::unique_ptr<GLProgram> program;

static const int NUM_MESHES = 4;

void update() {
    float width = app->getWindowSize().getWidth();
    float height = app->getWindowSize().getHeight();
    
    glm::mat4 modelMat, viewMat, projMat;
    
    glDepthMask(GL_FALSE);
    skybox->init(skyboxProgram->getID());
    skyboxProgram->use();
    skyboxProgram->setDefaultMats();
    cubeMap->bind();
    skybox->draw();
    glDepthMask(GL_TRUE);
    
    static glm::vec3 translations[NUM_MESHES] = {
        glm::vec3(-3.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -10.0f),
        glm::vec3(8.0f, 0.0f, -30.0f),
        glm::vec3(20.0f, 0.0f, -50.0f)
    };
    
    for (int i = 0; i < 4; i++) {
        brick->init(program->getID());
        program->use();
        modelMat = glm::translate(glm::mat4(), translations[i]);
        viewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        projMat = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
        program->setMat(modelMat, GLProgram::MatType::MODEL);
        program->setMat(viewMat, GLProgram::MatType::VIEW);
        program->setMat(projMat, GLProgram::MatType::PROJ);
        program->setMat4("projMat", projMat);
        program->setVec3("cameraPosition", glm::vec3(0.0f, 0.0f, 100.0f));
        texture->bind();
        brick->draw();
    }
}

int main() {
    app = std::make_unique<GLApplication>("Mipmappings", 1200, 750);
    
    // Skybox
    
    skyboxProgram = std::make_unique<GLProgram>();
    skyboxProgram->create();
    skyboxProgram->addShader("shaders/skybox_vs.glsl", GLProgram::ShaderType::VERTEXT);
    skyboxProgram->addShader("shaders/skybox_fs.glsl", GLProgram::ShaderType::FRAGMENT);
    skyboxProgram->link();
    
    skybox = std::make_unique<Skybox>();
    
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
    
    program = std::make_unique<GLProgram>();
    program->create();
    program->addShader("shaders/vs.glsl", GLProgram::ShaderType::VERTEXT);
    program->addShader("shaders/fs.glsl", GLProgram::ShaderType::FRAGMENT);
    program->link();
    
    std::vector<glm::vec3> vertices = {
        glm::vec3(-1.0, 1.0, 0.0),
        glm::vec3(-1.0, -1.0, 0.0),
        glm::vec3(1.0, -1.0, 0.0),
        glm::vec3(1.0, 1.0, 0.0),
    };
    std::vector<glm::vec2> textureCoords = {
        glm::vec2(0.0, 1.0),
        glm::vec2(0.0, 0.0),
        glm::vec2(1.0, 0.0),
        glm::vec2(1.0, 1.0),
    };
    std::vector<GLuint> indices = {
        0, 1, 2,
        0, 2, 3
    };
    brick = std::make_unique<Mesh>(vertices, std::vector<glm::vec3>(), textureCoords, indices);

    texture = std::make_unique<Texture>("assets/images/brickwall.jpg");
    
    app->mainLoop(update);
    
    return 0;
}
