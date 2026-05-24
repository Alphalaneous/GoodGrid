#include "../include/GradientColor.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace good_grid;

class GradientColor::Impl final {
public:
    cocos2d::ccColor4B m_colorA;
    cocos2d::ccColor4B m_colorB;
    bool m_hasColorB = false;
};

GradientColor::GradientColor() : m_impl(std::make_shared<Impl>()) {}

GradientColor::GradientColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) : m_impl(std::make_shared<Impl>()) {
    m_impl->m_colorA = ccColor4B{r, g, b, a};
}

GradientColor::GradientColor(const cocos2d::ccColor4B& colorA) : m_impl(std::make_shared<Impl>()) {
    m_impl->m_colorA = colorA;
}

GradientColor::GradientColor(const cocos2d::ccColor4B& colorA, const cocos2d::ccColor4B& colorB) : m_impl(std::make_shared<Impl>()) {
    m_impl->m_colorA = colorA;
    m_impl->m_colorB = colorB;
    m_impl->m_hasColorB = true;
}

GradientColor::~GradientColor() {}

GradientColor::operator cocos2d::ccColor4B() const {
    return m_impl->m_colorA;
}

cocos2d::ccColor4B GradientColor::getColorA() const {
    return m_impl->m_colorA;
}

cocos2d::ccColor4B GradientColor::getColorB() const {
    return m_impl->m_hasColorB ? m_impl->m_colorB : m_impl->m_colorA;
}