//
//  utilities.c
//  Lab1.2
//
//  Created by PointerFLY on 14/10/2017.
//  Copyright © 2017 PointerFLY. All rights reserved.
//

#include "utils.hpp"
#include <GLUT/glut.h>
#include <iostream>

using namespace std;

mat4 utils::Ortho(float left, float right,
           float bottom, float top,
           float near, float far) {
    
    mat4 proj = identity_mat4();
    proj = scale(proj,
                 vec3(2.0f / (right - left),
                      2.0f / (top - bottom),
                      -2.0f / (far - near)));
    proj = translate(proj,
                     vec3(-(left + right) / (right - left),
                          -(top + bottom) / (top - bottom),
                          -(far + near) / (far - near)));
    return proj;
}

void utils::DumpGLInfo() {
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* render = glGetString(GL_RENDERER);
    const GLubyte* gl_version = glGetString(GL_VERSION);
    const GLubyte* glu_version = gluGetString(GLU_VERSION);
    
    cout << "OpenGL Vendor: " << vendor << endl;
    cout << "Render: " << render << endl;
    cout << "OpenGl Version: " << gl_version << endl;
    cout << "GLUT Version: " << glu_version << endl;
}
