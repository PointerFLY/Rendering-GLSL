//
//  GLApplication.cpp
//  Rendering-GLSL
//
//  Created by PointerFLY on 05/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#include "GLApplication.hpp"
#include <cstdlib>
#include <iostream>
#include <GL/glew.h>

GLApplication::GLApplication(const std::string& title, int width, int height) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    
    _window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    _glContext = SDL_GL_CreateContext(_window);
    
    GLenum res = glewInit();
    if(res != GLEW_OK) {
        std::cerr << "Glew failed to initialize!" << std::endl;
    }
    
    glEnable(GL_DEPTH_TEST);
}

void GLApplication::mainLoop(void (*callback)()) {
    SDL_Event event;
    bool isRuning = true;
    while (isRuning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRuning = false;
            }
            
            callback();
            display();
            SDL_Delay(10);
        }
    }
}

GLApplication::~GLApplication() {
    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}

void GLApplication::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(_window);
}


