//
//  ShaderProgram.cpp
//  Assignment1
//
//  Created by PointerFLY on 09/11/2017.
//  Copyright © 2017 PointerFLY. All rights reserved.
//

#include "shader_program.hpp"
#include <fstream>
#include <sstream>

using namespace std;

void ShaderProgram::Create() {
    id_ = glCreateProgram();
    if (id_ == 0) {
        cerr << "Error creating shader program" << endl;
        exit(1);
    }
}

void ShaderProgram::AddShader(const string& file, GLenum shader_type) {
    GLuint shader_obj = glCreateShader(shader_type);
    if (!shader_obj) {
        cerr << "Error createing shader " << file << endl;
        exit(1);
    }
    
    string shader_str = ShaderProgram::ReadShaderFile(file);
    const char* c_str = shader_str.c_str();
    glShaderSource(shader_obj, 1, static_cast<const GLchar**>(&c_str), nullptr);
    glCompileShader(shader_obj);
    
    GLint success;
    glGetShaderiv(shader_obj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar error_msg[1024];
        glGetShaderInfoLog(shader_obj, sizeof(error_msg), nullptr, error_msg);
        cerr << "Error compiling shader " << file << ": " << error_msg << endl;
        exit(1);
    }
    
    glAttachShader(id_, shader_obj);
}

void ShaderProgram::Link() {
    GLint success = GL_FALSE;
    // After compiling all shader objects and attaching them to the program, we can finally link it
    glLinkProgram(id_);
    // Check for program related errors using glGetProgramiv
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar error_msg[1024] = { 0 };
        glGetProgramInfoLog(id_, sizeof(error_msg), nullptr, error_msg);
        cerr << "Invalid shader program: " << error_msg << endl;
        exit(1);
    }
}

void ShaderProgram::Use() {
    // Needs to be validated to check whether the program can execute given the current pipeline state
    GLint success = GL_FALSE;
    glValidateProgram(id_);
    // Check for program related errors using glGetProgramiv
    glGetProgramiv(id_, GL_VALIDATE_STATUS, &success);
    if (!success) {
        GLchar error_msg[1024] = { 0 };
        glGetProgramInfoLog(id_, sizeof(error_msg), NULL, error_msg);
        cerr << "Invalid shader program: " << error_msg << endl;
        exit(1);
    }
    
    // Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
    glUseProgram(id_);
}

void ShaderProgram::Delete() {
    glDeleteProgram(id_);
}

string ShaderProgram::ReadShaderFile(const string& file) {
    ifstream file_stream(file.c_str());
    if(file_stream.fail()) {
        cerr << "Error loading shader called: " << file << endl;
        exit(1);
    }
    
    stringstream stream;
    stream << file_stream.rdbuf();
    file_stream.close();
    
    return stream.str();
}


