//
//  Size.hpp
//  Rendering-GLSL
//
//  Created by PointerFLY on 07/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#ifndef Size_hpp
#define Size_hpp

template <typename T>
class Size {
public:
    Size():
    _width(0.0f),
    _height(0.0f) {
    }
    Size(const T& width, const T& height) {
        _width = width;
        _height = height;
    }
    const T& getWidth() const { return _width; };
    const T& getHeight() const { return _height; };
    void setWidth(const T& width) { _width = width; }
    void setHeight(const T& height) { _height = height; }
    
private:
    T _width;
    T _height;
};

#endif /* Size_hpp */
