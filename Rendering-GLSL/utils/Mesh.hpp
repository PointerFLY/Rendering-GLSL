//
//  Mesh.hpp
//  Rendering-GLSL
//
//  Created by PointerFLY on 05/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#ifndef Mesh_hpp
#define Mesh_hpp

#include <string>
#include <glm/glm.hpp>

class Mesh {
public:
    Mesh(const std::string& fileName);
    virtual void draw();
    
public:
    virtual const glm::vec3& getPosition() const { return _position; }
    virtual const glm::vec2& getTextureCoordinate() const { return _textureCoordinate; }
    virtual const glm::vec3& getNormal() const { return _normal; }
    
private:
    glm::vec3 _position;
    glm::vec2 _textureCoordinate;
    glm::vec3 _normal;
    std::string _fileName;
};

#endif /* Mesh_hpp */
