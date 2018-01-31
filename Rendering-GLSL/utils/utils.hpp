//
//  utilities.h
//  Lab1.2
//
//  Created by PointerFLY on 14/10/2017.
//  Copyright © 2017 PointerFLY. All rights reserved.
//

#ifndef utilities_h
#define utilities_h

#include "maths_funcs.h"

namespace utils {
    void DumpGLInfo();
    mat4 Ortho(float left, float right,
               float bottom, float top,
               float near, float far);
};

#endif /* utilities_h */
