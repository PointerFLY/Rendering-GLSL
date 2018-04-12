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
static std::unique_ptr<GLProgram> program;
static std::unique_ptr<Texture> brushTexture;
static std::unique_ptr<Texture> inkTexture;
static std::vector<std::shared_ptr<Mesh>> meshes;

static glm::vec3 cameraPosition(0.0f, 0.0f, 20.0f);
static glm::vec2 rotation;

void update() {
    glClearColor(0.6, 0.6, 0.6, 1.0);
    
    float width = app->getWindowSize().getWidth();
    float height = app->getWindowSize().getHeight();
    glm::mat4 modelMat, viewMat, projMat;
    
    for (auto& mesh: meshes) {
        mesh->init(program->getID());
        program->use();
        modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        viewMat = glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        projMat = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 1000.0f);
        
        program->setMat4("World", modelMat);
        program->setMat4("View", viewMat);
        program->setMat4("Projection", projMat);
        program->setVec3("DirectionalLight.Base.Color", glm::vec3(1.0f, 1.0f, 1.0f));
        program->setFloat("DirectionalLight.Base.AmbientIntensity", 0.25f);
        program->setFloat("DirectionalLight.Base.DiffuseIntensity", 0.35f);
        program->setVec3("DirectionalLight.Direction", glm::vec3(-1.0f, -1.0f, -1.0f));
        program->setVec3("CameraPosition", cameraPosition);
        program->setFloat("SpecularIntensity", 10);
        program->setFloat("SpecularPower", 100);
        
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
    
    program = std::make_unique<GLProgram>();
    program->create();
    program->addShader("shaders/inkwash_vs.glsl", GLProgram::ShaderType::VERTEXT);
    program->addShader("shaders/inkwash_gs.glsl", GLProgram::ShaderType::GEOMETRY);
    program->addShader("shaders/inkwash_fs.glsl", GLProgram::ShaderType::FRAGMENT);
    program->link();
    
    meshes = Mesh::createFromFile("assets/model/teapot.dae");
    brushTexture = std::make_unique<Texture>("assets/images/flower.png");
    inkTexture = std::make_unique<Texture>("assets/images/flower.png");

    app->mainLoop(update);
    
    return 0;
}
