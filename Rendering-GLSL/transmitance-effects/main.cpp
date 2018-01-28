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
#include "ShaderProgram.hpp"
#include "maths_funcs.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

const int kWidth = 800;
const int kHeight = 600;

ShaderProgram program;

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

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(kWidth, kHeight);
    glutInitWindowPosition(400, 100);
    glutCreateWindow("Transmitance Effects");
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
