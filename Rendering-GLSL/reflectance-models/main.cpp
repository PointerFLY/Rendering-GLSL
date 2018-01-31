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
#include "shader_program.hpp"
#include "maths_funcs.h"
#include <GLUT/GLUT.h>

using namespace std;

int window_width_ = 1200;
int window_height_ = 750;

ShaderProgram normal_program_;
ShaderProgram phong_program_;
ShaderProgram toon_program_;
ShaderProgram minnaert_program_;

float rotation_x_;
float rotation_y_;

GLuint vertex_vbo_;
GLuint normal_vbo_;

void SetupBuffers() {
    glGenBuffers(1, &vertex_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_);
    glBufferData(GL_ARRAY_BUFFER, 3 * teapot_vertex_count * sizeof(float), teapot_vertex_points, GL_STATIC_DRAW);
    glGenBuffers(1, &normal_vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_);
    glBufferData(GL_ARRAY_BUFFER, 3 * teapot_vertex_count * sizeof(float), teapot_normals, GL_STATIC_DRAW);
}

void CreatePrograms() {
    normal_program_.Create();
    normal_program_.AddShader("shaders/normal_vs.glsl", GL_VERTEX_SHADER);
    normal_program_.AddShader("shaders/normal_fs.glsl", GL_FRAGMENT_SHADER);
    normal_program_.Link();
    
    phong_program_.Create();
    phong_program_.AddShader("shaders/phong_vs.glsl", GL_VERTEX_SHADER);
    phong_program_.AddShader("shaders/phong_fs.glsl", GL_FRAGMENT_SHADER);
    phong_program_.Link();
    
    minnaert_program_.Create();
    minnaert_program_.AddShader("shaders/minnaert_vs.glsl", GL_VERTEX_SHADER);
    minnaert_program_.AddShader("shaders/minnaert_fs.glsl", GL_FRAGMENT_SHADER);
    minnaert_program_.Link();

    toon_program_.Create();
    toon_program_.AddShader("shaders/toon_vs.glsl", GL_VERTEX_SHADER);
    toon_program_.AddShader("shaders/toon_fs.glsl", GL_FRAGMENT_SHADER);
    toon_program_.Link();
}

void SetupShader(const ShaderProgram& program) {
    GLint v_model_mat = glGetUniformLocation(program.id(), "model_mat");
    GLint v_view_mat = glGetUniformLocation(program.id(), "view_mat");
    GLint v_proj_mat = glGetUniformLocation(program.id(), "proj_mat");
    mat4 proj_mat = perspective(75.0, (float)window_width_ / window_height_, 0.1, 1000.0);
    mat4 view_mat = look_at(vec3(0.0, 0.0, 30.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0));
    mat4 model_mat = identity_mat4();
    model_mat = rotate_x_deg(model_mat, rotation_x_);
    model_mat = rotate_y_deg(model_mat, rotation_y_);
    glUniformMatrix4fv(v_proj_mat, 1, GL_FALSE, proj_mat.m);
    glUniformMatrix4fv(v_view_mat, 1, GL_FALSE, view_mat.m);
    glUniformMatrix4fv(v_model_mat, 1, GL_FALSE, model_mat.m);
    
    GLint v_position = glGetAttribLocation(program.id(), "position");
    glEnableVertexAttribArray(v_position);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_);
    glVertexAttribPointer(v_position, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    GLint v_normal = glGetAttribLocation(program.id(), "normal");
    glEnableVertexAttribArray(v_normal);
    glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_);
    glVertexAttribPointer(v_normal, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void DrawNormal() {
    normal_program_.Use();
    glViewport(0, window_height_ / 2, window_width_ / 2, window_height_ / 2);
    SetupShader(normal_program_);
    glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);
}

void DrawPhong() {
    phong_program_.Use();
    glViewport(window_width_ / 2, window_height_ / 2, window_width_ / 2, window_height_ / 2);
    SetupShader(phong_program_);
    glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);
}

void DrawToon() {
    toon_program_.Use();
    glViewport(0, 0, window_width_ / 2, window_height_ / 2);
    SetupShader(toon_program_);
    glDrawArrays(GL_TRIANGLES, 0, teapot_vertex_count);
}

void DrawMinaert() {
    minnaert_program_.Use();
    glViewport(window_width_ / 2, 0, window_width_ / 2, window_height_ / 2);
    SetupShader(minnaert_program_);
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

void Reshape(int width, int height) {
    window_width_ = width;
    window_height_ = height;
    glutPostRedisplay();
}

void SpecialFunc(int key, int, int) {
    static float delta = 2.5;
    switch (key) {
        case GLUT_KEY_UP:
            rotation_x_ -= delta;
            break;
        case GLUT_KEY_DOWN:
            rotation_x_ += delta;
            break;
        case GLUT_KEY_LEFT:
            rotation_y_ -= delta;
            break;
        case GLUT_KEY_RIGHT:
            rotation_y_ += delta;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width_, window_height_);
    glutInitWindowPosition(100, 0);
    glutCreateWindow("Reflectance Models");
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);
    glutSpecialFunc(SpecialFunc);
    
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        cerr << "Error: " << glewGetErrorString(res) << endl;
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        exit(1);
    }
    
    utils::DumpGLInfo();
    Init();
    glutMainLoop();
    return 0;
}


