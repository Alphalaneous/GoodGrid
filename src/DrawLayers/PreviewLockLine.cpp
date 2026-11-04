#include "../../include/DrawLayers/PreviewLockLine.hpp"

namespace alpha::grid {

class PreviewLockLine::Impl final {
public:
    Color m_lineColor = {255, 150, 0, 255};
    int m_lineColorPriority = 0;

    float m_lineWidth = 2.f;
    int m_lineWidthPriority = 0;
};

PreviewLockLine::PreviewLockLine() : m_impl(std::make_unique<Impl>()) {}

PreviewLockLine::~PreviewLockLine() {}

PreviewLockLine* PreviewLockLine::create() {
    auto ret = new PreviewLockLine();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool PreviewLockLine::init() {
    if (!CCNode::init()) return false;
    setID("preview-lock-line"_spr);
    return true;
}

void PreviewLockLine::draw(const cocos2d::CCRect& visibleRect) {
    auto editorLayer = getDrawGridLayer()->m_editorLayer;
    if (editorLayer->m_playbackMode != PlaybackMode::Not || editorLayer->m_previewPosition.x <= 0.f) return;
    
    drawLine({editorLayer->m_previewPosition.x, visibleRect.getMinY()}, {editorLayer->m_previewPosition.x, visibleRect.getMaxY()}, m_impl->m_lineColor, m_impl->m_lineWidth);
}

void PreviewLockLine::setLineColor(const Color& color) {
    m_impl->m_lineColor = color;
}

const Color& PreviewLockLine::getLineColor() const {
    return m_impl->m_lineColor;
}

void PreviewLockLine::setLineWidth(float width) {
    m_impl->m_lineWidth = width;
}

float PreviewLockLine::getLineWidth() const {
    return m_impl->m_lineWidth;
}

const Color& PreviewLockLine::getDefaultLineColor() {
    static Color defaultColor = {255, 150, 0, 255};
    return defaultColor;
}

float PreviewLockLine::getDefaultLineWidth() {
    return 2.f;
}

}