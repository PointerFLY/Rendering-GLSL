//
//  Texture.cpp
//  Rendering-GLSL
//
//  Created by PointerFLY on 06/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#include "Texture.hpp"
#include <iostream>
#include <SOIL.h>

Texture::Texture(const std::string& fileName) {
    int width, height, channels;
    unsigned char* data = SOIL_load_image(fileName.c_str(), &width, &height, &channels, 4);
    if (!data) {
        std::cerr << "Unable to load texture: " << fileName << std::endl;
        exit(EXIT_FAILURE);
    }
    
    glGenTextures(1, &_id);
    glBindTexture(GL_TEXTURE_2D, _id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    SOIL_free_image_data(data);
}

void Texture::generateMipmaps() {
    glBindTexture(GL_TEXTURE_2D, _id);
    glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture() {
    glDeleteTextures(1, &_id);
}

void Texture::bind(int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, _id);
}
