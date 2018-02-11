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
#include <glm/glm.hpp>

class GLProgram {
public:
    enum class ShaderType {
        VERTEXT = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
    };
    
    static const int NUM_MATS = 3;
    enum class MatType: int {
        MODEL = 0,
        VIEW,
        PROJ,
    };
    
public:
    GLProgram();
    virtual ~GLProgram();
    virtual void addShader(const std::string& fileName, ShaderType type);
    virtual void create();
    virtual void link();
    virtual void use();
    
    virtual void setMat(const glm::mat4& mat, MatType type);
    virtual const glm::mat4& getMat(MatType type);
    
    virtual GLuint getID() const { return _id; }

private:
    GLuint _id;
    glm::mat4 _mats[NUM_MATS];
    GLint _uniformMats[NUM_MATS];
    
private:
    void checkError(GLuint id, GLenum flag, bool isProgram, const std::string& errorMessage);
    std::string loadShader(const std::string& fileName);
};

#endif /* GLProgram_hpp */
