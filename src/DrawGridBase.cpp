#include "../include/DrawGridBase.hpp"
#include "DrawGridLayer.hpp"

using namespace good_grid;

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

void DrawGridBase::drawLine(const cocos2d::ccVertex2F& start, const cocos2d::ccVertex2F& end, const GradientColor& color, float width, BlendMode mode) {
    if (!m_impl->m_drawGridLayer) return;

    const float scale = m_impl->m_drawGridLayer->m_editorLayer->m_objectLayer->getScale();

    width /= scale;
    width /= CCEGLView::get()->m_fScaleX;

    float ax = start.x;
    float ay = start.y;
    float bx = end.x;
    float by = end.y;

    float dx = bx - ax;
    float dy = by - ay;

    float len2 = dx * dx + dy * dy;
    if (len2 < 1e-6f) return;

    float len = std::sqrt(len2);
    float invLen = 1.0f / len;

    dx *= invLen;
    dy *= invLen;

    float nx = -dy;
    float ny = dx;

    nx *= width * 0.5f;
    ny *= width * 0.5f;

    ccVertex2F v0{ ax + nx, ay + ny };
    ccVertex2F v1{ ax - nx, ay - ny };
    ccVertex2F v2{ bx + nx, by + ny };
    ccVertex2F v3{ bx - nx, by - ny };

    drawQuad(v0, v1, v2, v3, color, mode);
}

void DrawGridBase::drawRect(const cocos2d::CCRect& rect, const GradientColor& color, DrawGridBase::BlendMode mode) {
    float x = rect.getMinX();
    float y = rect.getMinY();
    float w = rect.size.width;
    float h = rect.size.height;

    ccVertex2F v0{ x, y };
    ccVertex2F v1{ x + w, y };
    ccVertex2F v2{ x + w, y + h };
    ccVertex2F v3{ x, y + h };

    drawQuad(v0, v1, v2, v3, color, mode);
}

void DrawGridBase::drawQuad(const ccVertex2F& v0, const ccVertex2F& v1, const ccVertex2F& v2, const ccVertex2F& v3, const GradientColor& color, BlendMode mode) {
    if (!m_impl->m_drawGridLayer) return;

    auto custom = m_impl->m_drawGridLayer->getCustom();
    auto& batch = custom->batchForMode(mode);

    batch.push_back({v0, color.getColorA(), {0, 1}});
    batch.push_back({v1, color.getColorA(), {0, 0}});
    batch.push_back({v2, color.getColorB(), {1, 1}});

    batch.push_back({v2, color.getColorB(), {1, 1}});
    batch.push_back({v1, color.getColorA(), {0, 0}});
    batch.push_back({v3, color.getColorB(), {1, 0}});
}

void DrawGridBase::drawRectOutline(const cocos2d::CCRect& rect, const GradientColor& color, float width, DrawGridBase::BlendMode mode) {
    if (!m_impl->m_drawGridLayer) return;

    auto custom = m_impl->m_drawGridLayer->getCustom();
    auto& batch = custom->batchForMode(mode);

    const float scale = m_impl->m_drawGridLayer->m_editorLayer->m_objectLayer->getScale();

    width /= scale;
    width /= CCEGLView::get()->m_fScaleX;

    float x = rect.getMinX();
    float y = rect.getMinY();
    float w = rect.size.width;
    float h = rect.size.height;

    float t = width * 0.5f;

    float ox0 = x - t;
    float oy0 = y - t;
    float ox1 = x + w + t;
    float oy1 = y + h + t;

    float ix0 = x;
    float iy0 = y;
    float ix1 = x + w;
    float iy1 = y + h;

    auto push = [&](float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
        batch.push_back({{x1, y1}, color.getColorA(), {0, 1}});
        batch.push_back({{x2, y2}, color.getColorA(), {0, 0}});
        batch.push_back({{x3, y3}, color.getColorB(), {1, 1}});

        batch.push_back({{x1, y1}, color.getColorB(), {1, 1}});
        batch.push_back({{x3, y3}, color.getColorA(), {0, 0}});
        batch.push_back({{x4, y4}, color.getColorB(), {1, 0}});
    };

    push(ox0, oy0, ox1, oy0, ix1, iy0, ix0, iy0);
    push(ox1, oy0, ox1, oy1, ix1, iy1, ix1, iy0);
    push(ox1, oy1, ox0, oy1, ix0, iy1, ix1, iy1);
    push(ox0, oy1, ox0, oy0, ix0, iy0, ix0, iy1);
}

void DrawGridBase::visit() {}

void DrawGridBase::onEnter() {
    CCNode::onEnter();
    auto parent = static_cast<MyDrawGridLayer*>(typeinfo_cast<DrawGridLayer*>(getParent()));
    if (!parent) return;

    m_impl->m_drawGridLayer = parent;
}

void DrawGridBase::draw(float minX, float maxX, float minY, float maxY) {}

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