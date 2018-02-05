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

GLProgram::GLProgram() {
    _id = glCreateProgram();
    glLinkProgram(_id);
    glValidateProgram(_id);
}

GLProgram::~GLProgram() {
    glDeleteProgram(_id);
}

void GLProgram::addShader(const std::string& shaderPath, ShaderType type) {
    GLenum glEnum = static_cast<GLenum>(type);
    GLuint shaderID = glCreateShader(glEnum);
    if (type == ShaderType::VERTEXT) {
        _vertextShaderID = shaderID;
    } else {
        _fragmentShaderID = shaderID;
    }
    if (!shaderID) {
        std::cerr << "Error creating shader type: " << glEnum << std::endl;
        exit(EXIT_FAILURE);
    }
    
    const std::string& shaderText = loadShader(shaderPath);
    const char* cstr = shaderText.c_str();
    GLint length = static_cast<GLuint>(shaderText.size());
    glShaderSource(shaderID, 1, static_cast<const GLchar**>(&cstr), &length);
    glCompileShader(shaderID);
    glAttachShader(_id, shaderID);
}

void GLProgram::use() {
    glUseProgram(_id);
}

std::string GLProgram::loadShader(const std::string& shaderPath) {
    std::ifstream fileStream;
    fileStream.open(shaderPath);
    
    if (!fileStream.is_open()) {
        std::cerr << "Load shader error!" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::string text;
    while (!fileStream.eof()) {
        std::getline(fileStream, text);
    }
    
    fileStream.close();

    return text;
}


