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

static std::unique_ptr<GLApplication> app;
static std::unique_ptr<GLProgram> program;
static std::unique_ptr<Mesh> mesh;
static std::unique_ptr<Texture> texture;

void update() {
    float width = app->getWindowSize().getWidth();
    float height = app->getWindowSize().getHeight();
    
    mesh->init(program->getID());
    program->use();

    glm::mat4 modelMat;
    glm::mat4 viewMat = glm::lookAt(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projMat = glm::perspective(glm::radians(75.0f), width / height, 0.1f, 1000.0f);
    program->setMat(modelMat, GLProgram::MatType::MODEL);
    program->setMat(viewMat, GLProgram::MatType::VIEW);
    program->setMat(projMat, GLProgram::MatType::PROJ);
    
    mesh->draw();
}
int main() {
    app = std::make_unique<GLApplication>("Reflectance Models", 800, 600);
    
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
    std::vector<GLint> indices;

    mesh = std::make_unique<Mesh>(positions, normals, textureCoords, indices);
    
    texture = std::make_unique<Texture>("assets/images/bricks.jpg");
    texture->bind();
    
    app->mainLoop(update);
    
    return 0;
}
