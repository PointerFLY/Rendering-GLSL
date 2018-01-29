//
//  ShaderProgram.hpp
//  Assignment1
//
//  Created by PointerFLY on 09/11/2017.
//  Copyright © 2017 PointerFLY. All rights reserved.
//

#ifndef ShaderProgram_hpp
#define ShaderProgram_hpp

#include <GL/glew.h>
#include <GLUT/glut.h>
#include <iostream>

class ShaderProgram {
public:
    static std::string ReadShaderFile(const std::string& file);
    GLuint GetID() const { return id_; }
    void AddShader(const std::string& file, GLenum shader_type);
    void Create();
    void Link();
    void Use();
    void Delete();
    
private:
    GLuint id_;
};

#endif /* ShaderProgram_hpp */
