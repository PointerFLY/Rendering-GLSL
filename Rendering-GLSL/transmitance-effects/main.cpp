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

enum TransmitanceEffectType {
    REFLECT = 0,
    REFRACT,
    FRESNEL,
    DISPERSE,
};
static const int NUM_PROGRAMS = 4;

static std::unique_ptr<GLApplication> app;
static std::unique_ptr<GLProgram> skyboxProgram;
static std::unique_ptr<Mesh> skybox;
static std::unique_ptr<Mesh> mesh;
static std::unique_ptr<CubeMap> cubeMap;
static std::unique_ptr<GLProgram> programs[NUM_PROGRAMS];

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
    
    static glm::vec3 translations[NUM_PROGRAMS] = {
        glm::vec3(-25.0f, 15.0f, -10.0f),
        glm::vec3(25.0f, 15.0f, -10.0f),
        glm::vec3(-25.0f, -15.0f, 0.0f),
        glm::vec3(25.0f, -15.0f, 0.0f),
    };
    for (int i = 0; i < NUM_PROGRAMS; i++) {
        mesh->init(programs[i]->getID());
        programs[i]->use();
        modelMat = glm::translate(glm::mat4(), translations[i]);
        modelMat = glm::rotate(modelMat, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        modelMat = glm::rotate(modelMat, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        viewMat = glm::lookAt(cameraPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        projMat = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 1000.0f);
        programs[i]->setMat(modelMat, GLProgram::MatType::MODEL);
        programs[i]->setMat(viewMat, GLProgram::MatType::VIEW);
        programs[i]->setMat(projMat, GLProgram::MatType::PROJ);
        programs[i]->setVec3("cameraPosition", glm::vec3(0.0f, 0.0f, 100.0f));
        cubeMap->bind();
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
    
    std::string vertextShaderNames[NUM_PROGRAMS] = {
        "shaders/reflect_vs.glsl",
        "shaders/refract_vs.glsl",
        "shaders/fresnel_vs.glsl",
        "shaders/disperse_vs.glsl"
    };
    std::string fragmentShaderNames[NUM_PROGRAMS] = {
        "shaders/reflect_fs.glsl",
        "shaders/refract_fs.glsl",
        "shaders/fresnel_fs.glsl",
        "shaders/disperse_fs.glsl"
    };
    for (int i = 0; i < NUM_PROGRAMS; i++) {
        programs[i] = std::make_unique<GLProgram>();
        programs[i]->create();
        programs[i]->addShader(vertextShaderNames[i], GLProgram::ShaderType::VERTEXT);
        programs[i]->addShader(fragmentShaderNames[i], GLProgram::ShaderType::FRAGMENT);
        programs[i]->link();
    }
    
    programs[0] = std::make_unique<GLProgram>();
    programs[0]->create();
    programs[0]->addShader("shaders/reflect_vs.glsl", GLProgram::ShaderType::VERTEXT);
    programs[0]->addShader("shaders/reflect_fs.glsl", GLProgram::ShaderType::FRAGMENT);
    programs[0]->link();
    
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
