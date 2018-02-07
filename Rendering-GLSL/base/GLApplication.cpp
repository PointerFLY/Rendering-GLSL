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
    _windowSize = Size<int>(width, height);
    _viewport = Viewport(0, 0, width, height);
    
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
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
        std::cerr << "Glew failed to initialize: " << glewGetErrorString(res) << std::endl;
        exit(EXIT_FAILURE);
    }
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void GLApplication::setViewport(const Viewport& viewport) {
    _viewport = viewport;
    glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
}

void GLApplication::mainLoop(void (*update)()) {
    SDL_Event event;
    bool isRuning = true;
    while (isRuning) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                isRuning = false;
            }
            _eventHandler(event);
            
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            update();
            
            SDL_GL_SwapWindow(_window);
            SDL_Delay(10);
            // TODO: VSync
        }
    }
}

GLApplication::~GLApplication() {
    SDL_GL_DeleteContext(_glContext);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}


