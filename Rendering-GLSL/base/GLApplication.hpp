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
#include "Size.hpp"

class GLApplication {
public:
    GLApplication(const std::string& title, int width, int height);
    virtual ~GLApplication();
    virtual void mainLoop(void (*update)());
    virtual const Size<int>& getWindowSize() const { return _windowSize; }
    
protected:
    SDL_Window* _window;
    SDL_GLContext _glContext;
    Size<int> _windowSize;
};

#endif /* GLApplication_hpp */
