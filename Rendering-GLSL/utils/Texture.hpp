//
//  Texture.hpp
//  Rendering-GLSL
//
//  Created by PointerFLY on 06/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <string>
#include <GL/glew.h>

class Texture {
public:
    Texture(const std::string& fileName);
    virtual void bind();
    
protected:
    virtual ~Texture();
    
private:
    Texture(const Texture& texture) {}
    void operator=(const Texture& texture) {}
    
    GLuint _texture;
};

#endif /* Texture_hpp */
