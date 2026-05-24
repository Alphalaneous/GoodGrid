#include "../../include/DrawLayers/PreviewLockLine.hpp"

using namespace good_grid;

class PreviewLockLine::Impl final {
public:
    GradientColor m_lineColor = { 255, 150, 0, 255 };
    int m_lineColorPriority = 0;

    float m_lineWidth = 2.0f;
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

void PreviewLockLine::draw(float minX, float maxX, float minY, float maxY) {
    auto editorLayer = getDrawGridLayer()->m_editorLayer;

    if (editorLayer->m_playbackMode != PlaybackMode::Not) return;
    if (editorLayer->m_previewPosition.x <= 0) return;
    const cocos2d::CCPoint& pos = editorLayer->m_previewPosition;
    drawLine({pos.x, minY}, {pos.x, maxY}, m_impl->m_lineColor, m_impl->m_lineWidth);
}

void PreviewLockLine::setLineColor(const GradientColor& color) {
    m_impl->m_lineColor = color;
}

const GradientColor& PreviewLockLine::getLineColor() const {
    return m_impl->m_lineColor;
}

void PreviewLockLine::setLineWidth(float width) {
    m_impl->m_lineWidth = width;
}

float PreviewLockLine::getLineWidth() const {
    return m_impl->m_lineWidth;
}