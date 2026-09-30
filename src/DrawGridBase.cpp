#include "../include/DrawGridBase.hpp"
#include "DrawGridLayer.hpp"
#include <numbers>

namespace alpha::grid {

class DrawGridBase::Impl final {
public:
    MyDrawGridLayer* m_drawGridLayer = nullptr;
};

DrawGridBase::DrawGridBase() : m_impl(std::make_unique<Impl>()) {}
DrawGridBase::~DrawGridBase() {}

DrawGridBase* DrawGridBase::create() {
    auto ret = new DrawGridBase();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void DrawGridBase::drawQuad(const ccVertex2F& v0, const ccVertex2F& v1, const ccVertex2F& v2, const ccVertex2F& v3, const Color& color, float angle) {
    if (!m_impl->m_drawGridLayer) return;

    auto custom = m_impl->m_drawGridLayer->getCustom();
    auto& batch = custom->batchForFunc(color.getBlendFunc());

    batch.push_back({v0, color.getColorA(), {0.f, 1.f}, angle});
    batch.push_back({v1, color.getColorA(), {0.f, 0.f}, angle});
    batch.push_back({v2, color.getColorB(), {1.f, 1.f}, angle});

    batch.push_back({v2, color.getColorB(), {1.f, 1.f}, angle});
    batch.push_back({v1, color.getColorA(), {0.f, 0.f}, angle});
    batch.push_back({v3, color.getColorB(), {1.f, 0.f}, angle});
}

void DrawGridBase::drawLine(const cocos2d::ccVertex2F& start, const cocos2d::ccVertex2F& end, const Color& color, float width, bool relative) {
    if (!m_impl->m_drawGridLayer) return;

    float scale = relative ? 1.f : m_impl->m_drawGridLayer->m_editorLayer->m_objectLayer->getScale();
    float rot = m_impl->m_drawGridLayer->m_editorLayer->m_gameState.m_cameraAngle;
    float lineAngle = std::atan2(end.y - start.y, end.x - start.x);

    float fullAngle = lineAngle + CC_DEGREES_TO_RADIANS(rot);

    constexpr auto halfPi = std::numbers::pi_v<float> / 2.f;

    float axisAngle = std::fmod(std::abs(fullAngle), halfPi);
    float deviation = std::min(axisAngle, halfPi - axisAngle);
    float widthModifier = halfPi * std::sin(deviation * 2.f);

    width += widthModifier;
    width /= (scale * CCEGLView::get()->m_fScaleX * 2.f);

    float ax = start.x;
    float ay = start.y;
    float bx = end.x;
    float by = end.y;

    float dx = bx - ax;
    float dy = by - ay;

    float len2 = dx * dx + dy * dy;
    if (len2 < 1e-6f) return;

    float len = std::sqrt(len2);
    float invLen = 1.f / len;

    dx *= invLen;
    dy *= invLen;

    float nx = -dy;
    float ny = dx;

    nx *= width;
    ny *= width;

    auto v0 = ccVertex2F{ax + nx, ay + ny};
    auto v1 = ccVertex2F{ax - nx, ay - ny};
    auto v2 = ccVertex2F{bx + nx, by + ny};
    auto v3 = ccVertex2F{bx - nx, by - ny};

    drawQuad(v0, v1, v2, v3, color, fullAngle);
}

void DrawGridBase::drawRect(const cocos2d::CCRect& rect, const Color& color) {
    float rot = m_impl->m_drawGridLayer->m_editorLayer->m_gameState.m_cameraAngle;
    float angle = CC_DEGREES_TO_RADIANS(rot);

    drawRectInternal(rect, color, angle);
}

void DrawGridBase::drawRectInternal(const cocos2d::CCRect& rect, const Color& color, float angle) {
    auto v0 = ccVertex2F{rect.getMinX(), rect.getMinY()};
    auto v1 = ccVertex2F{rect.getMinX(), rect.getMaxY()};
    auto v2 = ccVertex2F{rect.getMaxX(), rect.getMinY()};
    auto v3 = ccVertex2F{rect.getMaxX(), rect.getMaxY()};

    drawQuad(v0, v1, v2, v3, color, angle);
}

void DrawGridBase::drawRectOutline(const cocos2d::CCRect& rect, const Color& color, float width, bool relative) {
    if (!m_impl->m_drawGridLayer) return;

    float scale = relative ? 1.f : m_impl->m_drawGridLayer->m_editorLayer->m_objectLayer->getScale();
    float rot = m_impl->m_drawGridLayer->m_editorLayer->m_gameState.m_cameraAngle;
    float angle = CC_DEGREES_TO_RADIANS(rot);

    constexpr auto halfPi = std::numbers::pi_v<float> / 2.f;

    float axisAngle = std::fmod(std::abs(angle), halfPi);
    float deviation = std::min(axisAngle, halfPi - axisAngle);
    float widthModifier = halfPi * std::sin(deviation * 2.f);

    float scaleModifier = scale * CCEGLView::get()->m_fScaleX * 2.f;

    float scaledWidthModifier = widthModifier / scaleModifier;

    width += widthModifier;
    width /= scaleModifier;

    auto b = CCRect{rect.getMinX(), rect.getMinY(), rect.getMaxX() - rect.getMinX() - width + scaledWidthModifier, width};
    auto t = CCRect{rect.getMinX() + width - scaledWidthModifier, rect.getMaxY() - width, rect.getMaxX() - rect.getMinX() - width + scaledWidthModifier, width};
    auto r = CCRect{rect.getMaxX() - width, rect.getMinY(), width, rect.getMaxY() - rect.getMinY() - width + scaledWidthModifier};
    auto l = CCRect{rect.getMinX(), rect.getMinY() + width - scaledWidthModifier, width, rect.getMaxY() - rect.getMinY() - width + scaledWidthModifier};
    
    drawRectInternal(b, color, angle);
    drawRectInternal(t, color, angle);
    drawRectInternal(r, color, angle);
    drawRectInternal(l, color, angle);
}

void DrawGridBase::visit() {}

void DrawGridBase::onEnter() {
    CCNode::onEnter();
    auto parent = static_cast<MyDrawGridLayer*>(typeinfo_cast<DrawGridLayer*>(getParent()));
    if (!parent) return;

    m_impl->m_drawGridLayer = parent;
}

void DrawGridBase::draw(const CCRect& visibleRect) {}

cocos2d::CCSize DrawGridBase::getGridBoundsSize() {
    return m_impl->m_drawGridLayer->getCustom()->getGridBoundsSize();
}

cocos2d::CCPoint DrawGridBase::getGridBoundsOrigin() {
    return m_impl->m_drawGridLayer->getCustom()->getGridBoundsOrigin();
}

bool DrawGridBase::isObjectVisible(GameObject* object) {
    return m_impl->m_drawGridLayer->getCustom()->isObjectVisible(object);
}

const std::unordered_map<float, cocos2d::ccColor4B>& DrawGridBase::getTimeMarkers() {
    return m_impl->m_drawGridLayer->getCustom()->getTimeMarkers();
}

DrawGridLayer* DrawGridBase::getDrawGridLayer() {
    return m_impl->m_drawGridLayer;
}

}