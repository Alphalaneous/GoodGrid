#pragma once

#include <ccTypes.h>

namespace alpha::grid {

struct Vertex {
    cocos2d::CCPoint position;
    cocos2d::ccColor4B color;
    cocos2d::CCPoint uv;
    float angle;
};

}