//
//  Skybox.cpp
//  Rendering-GLSL
//
//  Created by PointerFLY on 12/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#include "Skybox.hpp"
#include <iostream>

static const std::vector<glm::vec3> SKYBOX_VERTICES = {
    glm::vec3(-1.0f, -1.0f, 1.0f),
    glm::vec3(1.0f, -1.0f, 1.0f),
    glm::vec3(-1.0f, 1.0f, 1.0f),
    glm::vec3(1.0f, 1.0f, 1.0f),
    glm::vec3(-1.0f, -1.0f, -1.0f),
    glm::vec3(1.0f, -1.0f, -1.0f),
    glm::vec3(-1.0f, 1.0f, -1.0f),
    glm::vec3(1.0f, 1.0f, -1.0f)
};

static const std::vector<GLint> SKYBOX_INDICES = {
    5, 1, 3,
    3, 7, 5,
    2, 0, 4,
    4, 6, 2,
    3, 2, 6,
    6, 7, 3,
    4, 0, 1,
    1, 5, 4,
    6, 4, 5,
    5, 7, 6,
    0, 2, 3,
    3, 1, 0
};

Skybox::Skybox():
Mesh(SKYBOX_VERTICES, std::vector<glm::vec3>(), std::vector<glm::vec2>(), SKYBOX_INDICES) {
}

