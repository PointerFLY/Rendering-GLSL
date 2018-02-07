//
//  main.cpp
//  Lab2
//
//  Created by PointerFLY on 28/01/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#include <iostream>
#include "GLApplication.hpp"
#include "GLProgram.hpp"
#include "teapot.h"
#include "maths_funcs.h"
#include "Mesh.hpp"

std::unique_ptr<GLProgram> program;
std::unique_ptr<Mesh> mesh;

void update() {
    program->use();
    
    GLint vModelMat = glGetUniformLocation(program->getID(), "modelMat");
    GLint vViewMat = glGetUniformLocation(program->getID(), "viewMat");
    GLint vProjMat = glGetUniformLocation(program->getID(), "projMat");
    mat4 projMat = perspective(75.0, (float)800 / 600, 0.1, 1000.0);
    mat4 viewMat = look_at(vec3(0.0, 0.0, 30.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
    mat4 modelMat = identity_mat4();
    glUniformMatrix4fv(vProjMat, 1, GL_FALSE, projMat.m);
    glUniformMatrix4fv(vViewMat, 1, GL_FALSE, viewMat.m);
    glUniformMatrix4fv(vModelMat, 1, GL_FALSE, modelMat.m);
    
    mesh->draw();
}
int main() {
    GLApplication app("Transmitance Effects", 800, 600);
    
    program = std::make_unique<GLProgram>();
    program->create();
    program->addShader("shaders/vs.glsl", GLProgram::ShaderType::VERTEXT);
    program->addShader("shaders/fs.glsl", GLProgram::ShaderType::FRAGMENT);
    program->link();
    
    glm::vec3* positionArray = reinterpret_cast<glm::vec3*>(teapot_vertex_points);
    glm::vec3* normalArray = reinterpret_cast<glm::vec3*>(teapot_normals);
    glm::vec2* textureCoordsArray = reinterpret_cast<glm::vec2*>(teapot_tex_coords);
    std::vector<glm::vec3> positions(positionArray, positionArray + static_cast<size_t>(teapot_vertex_count));
    std::vector<glm::vec3> normals(normalArray, normalArray + static_cast<size_t>(teapot_vertex_count));
    std::vector<glm::vec2> textureCoords(textureCoordsArray, textureCoordsArray + static_cast<size_t>(teapot_vertex_count));
    std::vector<GLint> indices;

    mesh = std::make_unique<Mesh>(positions, normals, textureCoords, indices);
    mesh->init(program->getID());
    
    app.mainLoop(update);
}
