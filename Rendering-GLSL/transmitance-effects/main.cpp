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

GLProgram program;

GLuint vertexVBO;
GLuint normalVBO;

void bindVAO();

void setupBuffers() {
    bindVAO();
    
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * teapot_vertex_count * sizeof(float), teapot_vertex_points, GL_STATIC_DRAW);
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, 3 * teapot_vertex_count * sizeof(float), teapot_normals, GL_STATIC_DRAW);
}

void update() {
    program.use();

    GLint vModelMat = glGetUniformLocation(program.getID(), "model_mat");
    GLint vViewMat = glGetUniformLocation(program.getID(), "view_mat");
    GLint vProjMat = glGetUniformLocation(program.getID(), "proj_mat");
    mat4 projMat = perspective(75.0, (float)800 / 600, 0.1, 1000.0);
    mat4 viewMat = look_at(vec3(0.0, 0.0, 30.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
    mat4 modelMat = identity_mat4();
    glUniformMatrix4fv(vProjMat, 1, GL_FALSE, projMat.m);
    glUniformMatrix4fv(vViewMat, 1, GL_FALSE, viewMat.m);
    glUniformMatrix4fv(vModelMat, 1, GL_FALSE, modelMat.m);
    
    GLint vPosition = glGetAttribLocation(program.getID(), "position");
    glEnableVertexAttribArray(vPosition);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    GLint vNormal = glGetAttribLocation(program.getID(), "normal");
    glEnableVertexAttribArray(vNormal);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);
}

void bindVAO() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

int main() {
    GLApplication app("Transmitance Effects", 800, 600);

    program.create();
    program.addShader("shaders/vs.glsl", GLProgram::ShaderType::VERTEXT);
    program.addShader("shaders/fs.glsl", GLProgram::ShaderType::FRAGMENT);
    program.link();
    
    setupBuffers();
    
    app.mainLoop(update);
}
