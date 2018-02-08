//
//  CubeMap.cpp
//  Rendering-GLSL
//
//  Created by PointerFLY on 07/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#include "CubeMap.hpp"
#include <SOIL.h>
#include <iostream>

CubeMap::CubeMap(const std::vector<std::string>& fileNames) {
    for (int i = 0; i < fileNames.size(); i++) {
        int width, height, channels;
        unsigned char* data = SOIL_load_image(fileNames[i].c_str(), &width, &height, &channels, 0);
        if (!data) {
            std::cerr << "Unable to load CubeMap image: " << fileNames[i] << std::endl;
            exit(EXIT_FAILURE);
        }
        
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        SOIL_free_image_data(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

CubeMap::~CubeMap() {
    glDeleteTextures(1, &_id);
}

void CubeMap::bind() {
    glBindTexture(GL_TEXTURE_2D, _id);
}
