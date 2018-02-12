//
//  CubeMap.hpp
//  Rendering-GLSL
//
//  Created by PointerFLY on 07/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#ifndef CubeMap_hpp
#define CubeMap_hpp

#include <string>
#include <vector>
#include <GL/glew.h>

class CubeMap {
public:
    /// Image order: right, left, top, bottom, back, front.
    CubeMap(const std::vector<std::string>& fileNames);
    virtual ~CubeMap();
    virtual void bind();
    
public:
    virtual GLuint getID() const { return _id; }
    
protected:
    GLuint _id;
};

#endif /* CubeMap_hpp */
