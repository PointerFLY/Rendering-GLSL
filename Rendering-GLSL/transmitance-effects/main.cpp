//
//  main.cpp
//  Lab2
//
//  Created by PointerFLY on 28/01/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#include <iostream>
#include "teapot.h"
#include "utils.hpp"
#include "shader_program.hpp"
#include "maths_funcs.h"
#include <GLUT/GLUT.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;

int window_width_ = 1200;
int window_height_ = 750;

ShaderProgram program_;

void BindVAO() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
}

void CreatePrograms() {
    
}

void SetupBuffers() {
    
}

void Draw() {
    
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

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(window_width_, window_height_);
    glutInitWindowPosition(100, 0);
    glutCreateWindow("Transmitance Effects");
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);
    
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return 1;
    }
    
    utils::DumpGLInfo();
    Init();
    glutMainLoop();
    return 0;
}
