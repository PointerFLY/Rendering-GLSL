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
#include "teapot.h"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Skybox.hpp"
#include "CubeMap.hpp"


static std::unique_ptr<GLApplication> app;
static std::unique_ptr<GLProgram> skyboxProgram;
static std::unique_ptr<Mesh> skybox;
static std::unique_ptr<Mesh> mesh;
static std::unique_ptr<CubeMap> cubeMap;
static std::unique_ptr<GLProgram> program;

static glm::vec3 cameraPosition(0.0f, 0.0f, 100.0f);
static glm::vec2 rotation;

void update() {
    float width = app->getWindowSize().getWidth();
    float height = app->getWindowSize().getHeight();
    
    glm::mat4 modelMat, viewMat, projMat;
    
    glDepthMask(GL_FALSE);
    skybox->init(skyboxProgram->getID());
    skyboxProgram->use();
    viewMat = glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projMat = glm::perspective(glm::radians(90.0f), width / height, 0.1f, 2.0f);
    skyboxProgram->setMat(viewMat, GLProgram::MatType::VIEW);
    skyboxProgram->setMat(projMat, GLProgram::MatType::PROJ);
    cubeMap->bind();
    skybox->draw();
    glDepthMask(GL_TRUE);
    
    mesh->init(program->getID());
    program->use();
    modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    viewMat = glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    projMat = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 1000.0f);
    program->setMat(modelMat, GLProgram::MatType::MODEL);
    program->setMat(viewMat, GLProgram::MatType::VIEW);
    program->setMat(projMat, GLProgram::MatType::PROJ);
    program->setVec3("cameraPosition", cameraPosition);
    cubeMap->bind();
    mesh->draw();
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
    app = std::make_unique<GLApplication>("Transmitance Effect", 1200, 750);
    app->setEventHandler(handleEvents);
    
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
