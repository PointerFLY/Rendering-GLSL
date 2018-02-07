//
//  main2.cpp
//  reflectance-models
//
//  Created by PointerFLY on 07/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "GLApplication.hpp"
#include "GLProgram.hpp"
#include "teapot.h"
#include "Mesh.hpp"
#include "Texture.hpp"

static const int NUM_PROGRAMS = 4;
enum RelectanceModelType {
    NORMAL = 0,
    PHONG,
    TOON,
    MINNAERT,
};

static std::unique_ptr<GLApplication> app;
static std::unique_ptr<GLProgram> programs[NUM_PROGRAMS];
static std::unique_ptr<Mesh> mesh;

void update() {
    float width = app->getWindowSize().getWidth();
    float height = app->getWindowSize().getHeight();
    
    Viewport viewports[NUM_PROGRAMS] = {
        Viewport(0, height / 2, width / 2, height / 2),
        Viewport(width / 2, height / 2, width / 2, height / 2),
        Viewport(0, 0, width / 2, height / 2),
        Viewport(width / 2, 0, width / 2, height / 2)
    };
    
    for (int i = 0; i < NUM_PROGRAMS; i++) {
        mesh->init(programs[i]->getID());
        programs[i]->use();
        
        glm::mat4 modelMat;
        glm::mat4 viewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projMat = glm::perspective(glm::radians(75.0f), width / height, 0.1f, 1000.0f);
        programs[i]->setMat(modelMat, GLProgram::MatType::MODEL);
        programs[i]->setMat(viewMat, GLProgram::MatType::VIEW);
        programs[i]->setMat(projMat, GLProgram::MatType::PROJ);
        
        app->setViewport(viewports[i]);
        mesh->draw();
    }
}

int main() {
    app = std::make_unique<GLApplication>("Reflectance Models", 800, 600);
    
    std::string vertextShaderNames[NUM_PROGRAMS] = {
        "shaders/normal_vs.glsl",
        "shaders/phong_vs.glsl",
        "shaders/toon_vs.glsl",
        "shaders/minnaert_vs.glsl"
    };
    std::string fragmentShaderNames[NUM_PROGRAMS] = {
        "shaders/normal_fs.glsl",
        "shaders/phong_fs.glsl",
        "shaders/toon_fs.glsl",
        "shaders/minnaert_fs.glsl"
    };
    for (int i = 0; i < NUM_PROGRAMS; i++) {
        programs[i] = std::make_unique<GLProgram>();
        programs[i]->create();
        programs[i]->addShader(vertextShaderNames[i], GLProgram::ShaderType::VERTEXT);
        programs[i]->addShader(fragmentShaderNames[i], GLProgram::ShaderType::FRAGMENT);
        programs[i]->link();
    }
    
    static_assert(sizeof(glm::vec3) == sizeof(float) * 3, "sizeof(glm::vec3) != sizeof(float) * 3");
    glm::vec3* positionArray = reinterpret_cast<glm::vec3*>(teapot_vertex_points);
    glm::vec3* normalArray = reinterpret_cast<glm::vec3*>(teapot_normals);
    glm::vec2* textureCoordsArray = reinterpret_cast<glm::vec2*>(teapot_tex_coords);
    std::vector<glm::vec3> positions(positionArray, positionArray + static_cast<size_t>(teapot_vertex_count));
    std::vector<glm::vec3> normals(normalArray, normalArray + static_cast<size_t>(teapot_vertex_count));
    std::vector<glm::vec2> textureCoords(textureCoordsArray, textureCoordsArray + static_cast<size_t>(teapot_vertex_count));
    std::vector<GLint> indices;
    mesh = std::make_unique<Mesh>(positions, normals, textureCoords, indices);
    
    app->mainLoop(update);
    
    return 0;
}
