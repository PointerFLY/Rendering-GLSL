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
    virtual void addShader(const std::string& fileName, ShaderType type);
    virtual void create();
    virtual void link();
    virtual void use();
    
    virtual GLuint getID() const { return _id; }
    virtual GLuint getVertexShaderID() const { return _vertextShaderID; }
    virtual GLuint getFragmentShaderID() const { return _fragmentShaderID; }
    
    virtual const std::vector<GLint>& getUniforms() { return _uniforms; }
    virtual const std::vector<GLint>& getAttributes() { return _attributes; }

private:
    GLuint _id;
    GLuint _vertextShaderID;
    GLuint _fragmentShaderID;
    std::vector<GLint> _uniforms;
    std::vector<GLint> _attributes;
    
private:
    void checkError(GLuint id, GLenum flag, bool isProgram, const std::string& errorMessage);
    std::string loadShader(const std::string& fileName);
};

#endif /* GLProgram_hpp */
