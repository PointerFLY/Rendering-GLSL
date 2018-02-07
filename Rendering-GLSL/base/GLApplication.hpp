//
//  GLApplication.hpp
//  Rendering-GLSL
//
//  Created by PointerFLY on 05/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#ifndef GLApplication_hpp
#define GLApplication_hpp

#include <SDL2/SDL.h>
#include <string>
#include <GL/glew.h>
#include "Size.hpp"

struct Viewport {
    int x;
    int y;
    GLsizei width;
    GLsizei height;
    
    Viewport()
    : x(0), y(0), width(0), height(0) {
    }
    
    Viewport(int x, int y, GLsizei width, GLsizei height)
    : x(x), y(y), width(width), height(height) {
    }
};

class GLApplication {
public:
    GLApplication(const std::string& title, int width, int height);
    virtual ~GLApplication();
    virtual void mainLoop(void (*update)());
    virtual const Size<int>& getWindowSize() const { return _windowSize; }
    virtual void setViewport(const Viewport& viewport);
    virtual const Viewport& getViewport() { return _viewport; }
    
protected:
    SDL_Window* _window;
    SDL_GLContext _glContext;
    Size<int> _windowSize;
    Viewport _viewport;
};

#endif /* GLApplication_hpp */
