#pragma once

#include "Export.hpp"
#include <ccTypes.h>

namespace alpha::grid {

namespace blend {
    static constexpr cocos2d::ccBlendFunc Alpha = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA};
    static constexpr cocos2d::ccBlendFunc Additive = {GL_ONE, GL_ONE};
    static constexpr cocos2d::ccBlendFunc Multiply = {GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA};
    static constexpr cocos2d::ccBlendFunc Invert = {GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA};
}

class GOOD_GRID_API_DLL Color {
public:
    enum class BlendMode {
        Alpha,
        Additive,
        Multiply,
        Invert
    };

    Color();
    ~Color();

    Color(GLubyte r, GLubyte g, GLubyte b, GLubyte a, cocos2d::ccBlendFunc blendFunc = blend::Alpha);
    Color(const cocos2d::ccColor4B& colorA, cocos2d::ccBlendFunc blendFunc = blend::Alpha);
    Color(const cocos2d::ccColor4B& colorA, const cocos2d::ccColor4B& colorB, cocos2d::ccBlendFunc blendFunc = blend::Alpha);

    cocos2d::ccColor4B getColorA() const;
    cocos2d::ccColor4B getColorB() const;

    cocos2d::ccBlendFunc getBlendFunc() const;

    operator cocos2d::ccColor4B() const;
protected:

    class Impl;
    std::shared_ptr<Impl> m_impl;
};

}