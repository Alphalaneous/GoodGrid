#pragma once

#include <ccTypes.h>

namespace good_grid {

    struct Vertex {
        cocos2d::ccVertex2F position;
        cocos2d::ccColor4B color;
        cocos2d::ccVertex2F uv;
    };

}