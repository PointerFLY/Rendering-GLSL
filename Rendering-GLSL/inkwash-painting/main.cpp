//
//  main.cpp
//  inkwash-painting
//
//  Created by PointerFLY on 08/03/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "GLApplication.hpp"
#include "GLProgram.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "teapot.h"

static std::unique_ptr<GLApplication> app;
static std::unique_ptr<GLProgram> inkwashProgram;
static std::unique_ptr<GLProgram> normalProgram;
static std::unique_ptr<Texture> brushTexture;
static std::unique_ptr<Texture> inkTexture;
static std::vector<std::shared_ptr<Mesh>> meshes;

static glm::vec3 cameraPosition(0.0f, 0.0f, 15.0f);
static glm::vec2 rotation;

void update() {
    glClearColor(0.6, 0.6, 0.6, 1.0);
    
    float width = app->getWindowSize().getWidth();
    float height = app->getWindowSize().getHeight();
    glm::mat4 modelMat, viewMat, projMat;
    
    for (auto& mesh: meshes) {
        modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        viewMat = glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        projMat = glm::perspective(glm::radians(45.0f), 0.5f * width / height, 0.1f, 1000.0f);
        
        app->setViewport(Viewport(0, 0, width / 2.0, height));
        mesh->init(normalProgram->getID());
        normalProgram->use();
        normalProgram->setMat(modelMat, GLProgram::MatType::MODEL);
        normalProgram->setMat(viewMat, GLProgram::MatType::VIEW);
        normalProgram->setMat(projMat, GLProgram::MatType::PROJ);
        mesh->draw();
        
        app->setViewport(Viewport(width / 2.0, 0, width / 2.0, height));
        mesh->init(inkwashProgram->getID());
        inkwashProgram->use();
        inkwashProgram->setMat(modelMat, GLProgram::MatType::MODEL);
        inkwashProgram->setMat(viewMat, GLProgram::MatType::VIEW);
        inkwashProgram->setMat(projMat, GLProgram::MatType::PROJ);
        inkwashProgram->setVec3("cameraPosition", cameraPosition);
        inkwashProgram->setVec3("directionalLight.base.color", glm::vec3(1.0f, 1.0f, 1.0f));
        inkwashProgram->setFloat("directionalLight.base.ambientIntensity", 0.25f);
        inkwashProgram->setFloat("directionalLight.base.diffuseIntensity", 0.35f);
        inkwashProgram->setVec3("directionalLight.direction", glm::vec3(-1.0f, -1.0f, -1.0f));
        inkwashProgram->setFloat("specularIntensity", 10);
        inkwashProgram->setFloat("specularPower", 100);
        brushTexture->bind(0);
        inkTexture->bind(1);
        mesh->draw();
    }
}

void handleEvents(const SDL_Event& event) {
    const float delta = glm::radians(5.0f);
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                rotation.y -= delta;
                break;
            case SDLK_RIGHT:
                rotation.y += delta;
                break;
            case SDLK_UP:
                rotation.x -= delta;
                break;
            case SDLK_DOWN:
                rotation.x += delta;
                break;
            default:
                break;
        }
    }
}

int main() {
    app = std::make_unique<GLApplication>("Inkwash Painting", 1200, 750);
    app->setEventHandler(handleEvents);
    
    inkwashProgram = std::make_unique<GLProgram>();
    inkwashProgram->create();
    inkwashProgram->addShader("shaders/inkwash_vs.glsl", GLProgram::ShaderType::VERTEXT);
    inkwashProgram->addShader("shaders/inkwash_gs.glsl", GLProgram::ShaderType::GEOMETRY);
    inkwashProgram->addShader("shaders/inkwash_fs.glsl", GLProgram::ShaderType::FRAGMENT);
    inkwashProgram->link();
    
    normalProgram = std::make_unique<GLProgram>();
    normalProgram->create();
    normalProgram->addShader("shaders/normal_vs.glsl", GLProgram::ShaderType::VERTEXT);
    normalProgram->addShader("shaders/normal_fs.glsl", GLProgram::ShaderType::FRAGMENT);
    normalProgram->link();

    brushTexture = std::make_unique<Texture>("assets/images/brush.png");
    inkTexture = std::make_unique<Texture>("assets/images/ink.jpg");

    meshes = Mesh::createFromFile("assets/model/teapot.dae");
    
    app->mainLoop(update);
    
    return 0;
}
