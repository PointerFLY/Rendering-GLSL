//
//  main.cpp
//  tcd_rendering
//
//  Created by PointerFLY on 18/01/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#include <iostream>
#include "teapot.h"
#include "utils.hpp"
#include "ShaderProgram.hpp"
#include "maths_funcs.h"

const int kWidth = 800;
const int kHeight = 600;

ShaderProgram normal_program;
ShaderProgram phong_program;
ShaderProgram toon_program;
ShaderProgram minnaert_program;

void SetupBuffers() {    
    GLuint point_vbo;
    glGenBuffers(1, &point_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, point_vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * teapot_vertex_count * sizeof(float), teapot_vertex_points, GL_STATIC_DRAW);
    GLuint normal_vbo;
    glGenBuffers(1, &normal_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
    glBufferData(GL_ARRAY_BUFFER, 3 * teapot_vertex_count * sizeof(float), teapot_normals, GL_STATIC_DRAW);
    
    GLint v_position = glGetAttribLocation(phong_program.GetID(), "position");
    glEnableVertexAttribArray(v_position);
    glBindBuffer(GL_ARRAY_BUFFER, point_vbo);
    glVertexAttribPointer(v_position, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    GLint v_normal = glGetAttribLocation(phong_program.GetID(), "normal");
    glEnableVertexAttribArray(v_normal);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo);
    glVertexAttribPointer(v_normal, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void CreatePrograms() {
    normal_program.Create();
    normal_program.AddShader("shaders/normal_vs.glsl", GL_VERTEX_SHADER);
    normal_program.AddShader("shaders/normal_fs.glsl", GL_FRAGMENT_SHADER);
    normal_program.Link();
    
    phong_program.Create();
    phong_program.AddShader("shaders/phong_vs.glsl", GL_VERTEX_SHADER);
    phong_program.AddShader("shaders/phong_fs.glsl", GL_FRAGMENT_SHADER);
    phong_program.Link();
    
    minnaert_program.Create();
    minnaert_program.AddShader("shaders/minnaert_vs.glsl", GL_VERTEX_SHADER);
    minnaert_program.AddShader("shaders/minnaert_fs.glsl", GL_FRAGMENT_SHADER);
    minnaert_program.Link();

    toon_program.Create();
    toon_program.AddShader("shaders/toon_vs.glsl", GL_VERTEX_SHADER);
    toon_program.AddShader("shaders/toon_fs.glsl", GL_FRAGMENT_SHADER);
    toon_program.Link();
}

void DrawNormal() {
    normal_program.Use();
    glViewport(0, kHeight / 2, kWidth / 2, kHeight / 2);
    
    GLint v_model_mat = glGetUniformLocation(normal_program.GetID(), "model_mat");
    GLint v_view_mat = glGetUniformLocation(normal_program.GetID(), "view_mat");
    GLint v_proj_mat = glGetUniformLocation(normal_program.GetID(), "proj_mat");
    mat4 proj_mat = perspective(75.0, (float)kWidth / kHeight, 0.1, 1000.0);
    mat4 view_mat = look_at(vec3(0.0, 0.0, 30.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
    mat4 model_mat = identity_mat4();
    glUniformMatrix4fv(v_proj_mat, 1, GL_FALSE, proj_mat.m);
    glUniformMatrix4fv(v_view_mat, 1, GL_FALSE, view_mat.m);
    glUniformMatrix4fv(v_model_mat, 1, GL_FALSE, model_mat.m);
    
    glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);
}

void DrawPhong() {
    phong_program.Use();
    glViewport(kWidth / 2, kHeight / 2, kWidth / 2, kHeight / 2);
    
    GLint v_model_mat = glGetUniformLocation(phong_program.GetID(), "model_mat");
    GLint v_view_mat = glGetUniformLocation(phong_program.GetID(), "view_mat");
    GLint v_proj_mat = glGetUniformLocation(phong_program.GetID(), "proj_mat");
    mat4 proj_mat = perspective(75.0, (float)kWidth / kHeight, 0.1, 1000.0);
    mat4 view_mat = look_at(vec3(0.0, 0.0, 30.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
    mat4 model_mat = identity_mat4();
    glUniformMatrix4fv(v_proj_mat, 1, GL_FALSE, proj_mat.m);
    glUniformMatrix4fv(v_view_mat, 1, GL_FALSE, view_mat.m);
    glUniformMatrix4fv(v_model_mat, 1, GL_FALSE, model_mat.m);
    
    glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);
}

void DrawToon() {
    toon_program.Use();
    glViewport(0, 0, kWidth / 2, kHeight / 2);
    
    GLint v_model_mat = glGetUniformLocation(toon_program.GetID(), "model_mat");
    GLint v_view_mat = glGetUniformLocation(toon_program.GetID(), "view_mat");
    GLint v_proj_mat = glGetUniformLocation(toon_program.GetID(), "proj_mat");
    mat4 proj_mat = perspective(75.0, (float)kWidth / kHeight, 0.1, 1000.0);
    mat4 view_mat = look_at(vec3(0.0, 0.0, 30.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
    mat4 model_mat = identity_mat4();
    glUniformMatrix4fv(v_proj_mat, 1, GL_FALSE, proj_mat.m);
    glUniformMatrix4fv(v_view_mat, 1, GL_FALSE, view_mat.m);
    glUniformMatrix4fv(v_model_mat, 1, GL_FALSE, model_mat.m);
    
    glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);
}

void DrawMinaert() {
    minnaert_program.Use();
    glViewport(kWidth / 2, 0, kWidth / 2, kHeight / 2);
    
    GLint v_model_mat = glGetUniformLocation(minnaert_program.GetID(), "model_mat");
    GLint v_view_mat = glGetUniformLocation(minnaert_program.GetID(), "view_mat");
    GLint v_proj_mat = glGetUniformLocation(minnaert_program.GetID(), "proj_mat");
    mat4 proj_mat = perspective(75.0, (float)kWidth / kHeight, 0.1, 1000.0);
    mat4 view_mat = look_at(vec3(0.0, 0.0, 30.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
    mat4 model_mat = identity_mat4();
    glUniformMatrix4fv(v_proj_mat, 1, GL_FALSE, proj_mat.m);
    glUniformMatrix4fv(v_view_mat, 1, GL_FALSE, view_mat.m);
    glUniformMatrix4fv(v_model_mat, 1, GL_FALSE, model_mat.m);
    
    glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);
    
}

void Draw() {
    DrawNormal();
    DrawPhong();
    DrawToon();
    DrawMinaert();
}

void BindVAO() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

void Init() {
    BindVAO();
    CreatePrograms();
    SetupBuffers();
}

void Display() {
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Draw();
    glutSwapBuffers();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(kWidth, kHeight);
    glutInitWindowPosition(400, 100);
    glutCreateWindow("Lab 1");
    glutDisplayFunc(Display);
    
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }
    
    DumpGLInfo();
    Init();
    glutMainLoop();
    return 0;
}


