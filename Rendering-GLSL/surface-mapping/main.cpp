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

static std::unique_ptr<GLApplication> app;
static std::unique_ptr<GLProgram> program;
static std::unique_ptr<Mesh> brick;
static std::unique_ptr<Mesh> bumpBrick;
static std::unique_ptr<Texture> texture;
static std::unique_ptr<Texture> normalMap;

void update() {
    float width = app->getWindowSize().getWidth();
    float height = app->getWindowSize().getHeight();
    
    glm::mat4 modelMat, viewMat, projMat;
    program->setInt("sampler", 0);
    program->setInt("normalMap", 1);
    
    bumpBrick->init(program->getID());
    brick->init(program->getID());
    program->use();

    modelMat = glm::translate(modelMat, glm::vec3(-1.4f, 0.0f, 0.0f));
    viewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projMat = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
    program->setMat(modelMat, GLProgram::MatType::MODEL);
    program->setMat(viewMat, GLProgram::MatType::VIEW);
    program->setMat(projMat, GLProgram::MatType::PROJ);
    program->setInt("isBumped", false);
    texture->bind(0);
    normalMap->bind(1);
    brick->draw();
    
    modelMat = glm::translate(glm::mat4(), glm::vec3(1.4f, 0.0f, 0.0f));
    program->setMat(modelMat, GLProgram::MatType::MODEL);
    program->setInt("isBumped", true);
    texture->bind(0);
    normalMap->bind(1);
    bumpBrick->draw();
}

int main() {
    app = std::make_unique<GLApplication>("Reflectance Models", 1200, 750);
    
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
    bumpBrick = std::make_unique<Mesh>(vertices, std::vector<glm::vec3>(), textureCoords, indices);
    
    texture = std::make_unique<Texture>("assets/images/brickwall.jpg");
    normalMap = std::make_unique<Texture>("assets/images/brickwall_normal.jpg");
    
    app->mainLoop(update);
    
    return 0;
}
