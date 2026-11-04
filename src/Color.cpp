#include "../include/Color.hpp"
#include "ccTypes.h"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace alpha::grid {

class Color::Impl final {
public:
    cocos2d::ccColor4B m_colorA = {0, 0, 0, 255};
    cocos2d::ccColor4B m_colorB = {0, 0, 0, 255};
    bool m_hasColorB = false;
    ccBlendFunc m_blendFunc = blend::Additive;
};

Color::Color() : m_impl(std::make_shared<Impl>()) {}

Color::Color(GLubyte r, GLubyte g, GLubyte b, GLubyte a, ccBlendFunc blendFunc) : m_impl(std::make_shared<Impl>()) {
    m_impl->m_colorA = ccColor4B{r, g, b, a};
    m_impl->m_blendFunc = blendFunc;
}

Color::Color(const cocos2d::ccColor4B& colorA, ccBlendFunc blendFunc) : m_impl(std::make_shared<Impl>()) {
    m_impl->m_colorA = colorA;
    m_impl->m_blendFunc = blendFunc;
}

Color::Color(const cocos2d::ccColor4B& colorA, const cocos2d::ccColor4B& colorB, ccBlendFunc blendFunc) : m_impl(std::make_shared<Impl>()) {
    m_impl->m_colorA = colorA;
    m_impl->m_colorB = colorB;
    m_impl->m_hasColorB = true;
    m_impl->m_blendFunc = blendFunc;
}

Color::~Color() {}

Color::operator cocos2d::ccColor4B() const {
    return m_impl->m_colorA;
}

cocos2d::ccColor4B Color::getColorA() const {
    return m_impl->m_colorA;
}

cocos2d::ccColor4B Color::getColorB() const {
    return m_impl->m_hasColorB ? m_impl->m_colorB : m_impl->m_colorA;
}

ccBlendFunc Color::getBlendFunc() const {
    return m_impl->m_blendFunc;
}

}