#include "../../include/DrawLayers/Bounds.hpp"
#include "../DrawGridLayer.hpp"
#include "../Utils.hpp"

namespace alpha::grid {

class Bounds::Impl final {
public:
    Color m_topBoundColor = {255, 255, 255, 255};
    Color m_bottomBoundColor = {255, 255, 255, 255};
    Color m_verticalBoundColor = {255, 255, 255, 255};

    float m_topBoundLineWidth = 2.f;
    float m_bottomBoundLineWidth = 2.f;
    float m_verticalBoundLineWidth = 1.f;
};

Bounds::Bounds() : m_impl(std::make_unique<Impl>()) {}

Bounds::~Bounds() {}

Bounds* Bounds::create() {
    auto ret = new Bounds();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Bounds::init() {
    if (!CCNode::init()) return false;
    setID("bounds"_spr);
    return true;
}

void Bounds::draw(const cocos2d::CCRect& visibleRect) {
    drawLine({0.f, visibleRect.getMinY()}, {0.f, visibleRect.getMaxY()}, m_impl->m_verticalBoundColor, m_impl->m_verticalBoundLineWidth);

    if (!getDrawGridLayer()->m_editorLayer->m_showGround) {
        drawLine({visibleRect.getMinX(), DrawGridBase::GroundOffset}, {visibleRect.getMaxX(), DrawGridBase::GroundOffset}, m_impl->m_bottomBoundColor, m_impl->m_bottomBoundLineWidth);
    }

    auto levelSettings = alpha::grid::utils::getDrawGridLayer()->m_editorLayer->m_levelSettings;
    auto boundHeight = alpha::grid::utils::getDrawGridLayer()->getCustom()->getGridBoundsSize().height;

    float height = levelSettings->m_dynamicLevelHeight ? boundHeight : DrawGridBase::MaxHeight;

    if (visibleRect.getMinY() <= height || visibleRect.getMaxY() >= height) {
        drawLine({visibleRect.getMinX(), height}, {visibleRect.getMaxX(), height}, m_impl->m_topBoundColor, m_impl->m_topBoundLineWidth);
    }
}

void Bounds::setTopBoundColor(const Color& color) {
    m_impl->m_topBoundColor = color;
}

void Bounds::setBottomBoundColor(const Color& color) {
    m_impl->m_bottomBoundColor = color;
}

void Bounds::setVerticalBoundColor(const Color& color) {
    m_impl->m_verticalBoundColor = color;
}

const Color& Bounds::getTopBoundColor() const {
    return m_impl->m_topBoundColor;
}

const Color& Bounds::getBottomBoundColor() const {
    return m_impl->m_bottomBoundColor;
}

const Color& Bounds::getVerticalColor() const {
    return m_impl->m_verticalBoundColor;
}

void Bounds::setTopBoundLineWidth(float width) {
    m_impl->m_topBoundLineWidth = width;
}

void Bounds::setBottomBoundLineWidth(float width) {
    m_impl->m_bottomBoundLineWidth = width;
}

void Bounds::setVerticalBoundLineWidth(float width) {
    m_impl->m_verticalBoundLineWidth = width;
}

float Bounds::getTopBoundLineWidth() const {
    return m_impl->m_topBoundLineWidth;
}

float Bounds::getBottomBoundLineWidth() const {
    return m_impl->m_bottomBoundLineWidth;
}

float Bounds::getVerticalLineWidth() const {
    return m_impl->m_verticalBoundLineWidth;
}

}