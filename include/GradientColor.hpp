#pragma once

#include "Export.hpp"
#include <ccTypes.h>

namespace good_grid {

    class GOOD_GRID_API_DLL GradientColor {
    public:
        GradientColor();
        ~GradientColor();

        GradientColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
        GradientColor(const cocos2d::ccColor4B& colorA);
        GradientColor(const cocos2d::ccColor4B& colorA, const cocos2d::ccColor4B& colorB);

        cocos2d::ccColor4B getColorA() const;
        cocos2d::ccColor4B getColorB() const;

        operator cocos2d::ccColor4B() const;
    protected:

        class Impl;
        std::shared_ptr<Impl> m_impl;
    };

}