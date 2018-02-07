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

class GLApplication {
public:
    GLApplication(const std::string& title, int width, int height);
    virtual ~GLApplication();
    virtual void mainLoop(void (*update)());
    
protected:
    SDL_Window* _window;
    SDL_GLContext _glContext;
};

#endif /* GLApplication_hpp */
