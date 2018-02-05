//
//  GLProgram.hpp
//  Rendering-GLSL
//
//  Created by PointerFLY on 05/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#ifndef GLProgram_hpp
#define GLProgram_hpp

#include <GL/glew.h>
#include <vector>

class GLProgram {
public:
    enum class ShaderType {
        VERTEXT = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
    };
    
public:
    GLProgram();
    virtual ~GLProgram();
    virtual void addShader(const std::string& shaderPath, ShaderType type);
    virtual void use();
    
    virtual GLuint getID() const { return _id; }
    virtual GLuint getVertexShaderID() const { return _vertextShaderID; }
    virtual GLuint getFragmentShaderID() const { return _fragmentShaderID; }

private:
    GLuint _id;
    GLuint _vertextShaderID;
    GLuint _fragmentShaderID;
    
private:
    std::string loadShader(const std::string& shaderPath);
};

#endif /* GLProgram_hpp */
