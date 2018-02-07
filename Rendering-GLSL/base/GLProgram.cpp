//
//  GLProgram.cpp
//  Rendering-GLSL
//
//  Created by PointerFLY on 05/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#include "GLProgram.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>

GLProgram::GLProgram() {
    
}

GLProgram::~GLProgram() {
    if (_id != 0) {
        glDeleteProgram(_id);
    }
}

void GLProgram::create() {
    _id = glCreateProgram();
    if (!_id) {
        std::cerr << "Error creating shader program" << std::endl;
        exit(EXIT_FAILURE);
    }
}

void GLProgram::link() {
    glLinkProgram(_id);
    checkError(_id, GL_LINK_STATUS, true, "Invalid shader program: ");
    
    _uniformMats[static_cast<int>(MatType::MODEL)] = glGetUniformLocation(_id, "modelMat");
    _uniformMats[static_cast<int>(MatType::VIEW)] = glGetUniformLocation(_id, "viewMat");
    _uniformMats[static_cast<int>(MatType::PROJ)] = glGetUniformLocation(_id, "projMat");
}

void GLProgram::addShader(const std::string& fileName, ShaderType type) {
    GLenum glEnum = static_cast<GLenum>(type);
    GLuint shaderID = glCreateShader(glEnum);
    if (type == ShaderType::VERTEXT) {
        glDetachShader(_id, _vertextShaderID);
        _vertextShaderID = shaderID;
    } else {
        glDetachShader(_id, _fragmentShaderID);
        _fragmentShaderID = shaderID;
    }
    if (!shaderID) {
        std::cerr << "Error creating shader type: " << fileName << std::endl;
        exit(EXIT_FAILURE);
    }
    
    const std::string& shaderText = loadShader(fileName);
    const char* cstr = shaderText.c_str();
    GLint length = static_cast<GLuint>(shaderText.size());
    glShaderSource(shaderID, 1, static_cast<const GLchar**>(&cstr), &length);
    glCompileShader(shaderID);
    
    checkError(shaderID, GL_COMPILE_STATUS, false, "Error compiling shader " + fileName);
    glAttachShader(_id, shaderID);
}

void GLProgram::use() {
    glValidateProgram(_id);
    checkError(_id, GL_VALIDATE_STATUS, true, "Invalid shader program: ");
    glUseProgram(_id);
}

void GLProgram::setMat(const glm::mat4& mat, MatType type) {
    int idx = static_cast<int>(type);
    _mats[idx] = mat;
    glUniformMatrix4fv(_uniformMats[idx], 1, GL_FALSE, glm::value_ptr(mat));
}

const glm::mat4& GLProgram::getMat(MatType type) {
    int idx = static_cast<int>(type);
    return _mats[idx];
}

void GLProgram::checkError(GLuint id, GLuint flag, bool isProgram, const std::string& errorMessage) {
    GLint success = 0;
    GLchar error[1024] = { 0 };
    
    if(isProgram) {
        glGetProgramiv(id, flag, &success);
        glGetProgramInfoLog(id, sizeof(error), NULL, error);
    } else {
        glGetShaderiv(id, flag, &success);
        glGetShaderInfoLog(id, sizeof(error), NULL, error);
    }
    
    if(success == GL_FALSE) {
        std::cerr << errorMessage << error << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::string GLProgram::loadShader(const std::string& fileName) {
    std::ifstream file(fileName.c_str());
    if(file.fail()) {
        std::cout << "Error loading shader " << fileName << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    
    return stream.str();
}


