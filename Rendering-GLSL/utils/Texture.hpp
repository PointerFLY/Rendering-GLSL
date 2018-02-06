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

public:
    virtual GLuint getID() const { return _id; }
    
protected:
    virtual ~Texture();
    GLuint _id;
};

#endif /* Texture_hpp */
