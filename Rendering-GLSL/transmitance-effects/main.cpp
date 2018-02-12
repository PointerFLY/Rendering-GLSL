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
static std::unique_ptr<GLProgram> program;
static std::unique_ptr<Mesh> skybox;
static std::unique_ptr<CubeMap> cubeMap;

void update() {
    skybox->init(program->getID());
    program->use();
    program->setDefaultMats();
    cubeMap->bind();
    skybox->draw();
}

int main() {
    app = std::make_unique<GLApplication>("Reflectance Models", 1200, 750);
    
    program = std::make_unique<GLProgram>();
    program->create();
    program->addShader("shaders/skybox_vs.glsl", GLProgram::ShaderType::VERTEXT);
    program->addShader("shaders/skybox_fs.glsl", GLProgram::ShaderType::FRAGMENT);
    program->link();

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
    
    app->mainLoop(update);
    
    return 0;
}
