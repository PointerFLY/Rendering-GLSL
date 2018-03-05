//
//  main.cpp
//  surface-mapping
//
//  Created by PointerFLY on 08/02/2018.
//  Copyright © 2018 PointerFLY. All rights reserved.
//

#include <iostream>
#include "GLApplication.hpp"

static std::unique_ptr<GLApplication> app;

void update() {
    float width = app->getWindowSize().getWidth();
    float height = app->getWindowSize().getHeight();
}

int main() {
    app = std::make_unique<GLApplication>("Inkwash Painting", 1200, 750);
    
    app->mainLoop(update);
    
    return 0;
}
