#include "../../include/DrawLayers/Ground.hpp"

using namespace good_grid;

class Ground::Impl final {
public:
    GradientColor m_topGroundColor = { 255, 175, 50, 255 };
    GradientColor m_bottomGroundColor = { 255, 175, 50, 255 };

    float m_topGroundLineWidth = 2.0f;
    float m_bottomGroundLineWidth = 2.0f;

    float m_minPortalY = 0;
    float m_maxPortalY = 0;
};

Ground::Ground() : m_impl(std::make_unique<Impl>()) {}

Ground::~Ground() {}

Ground* Ground::create() {
    auto ret = new Ground();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Ground::init() {
    if (!CCNode::init()) return false;
    setID("ground"_spr);
    return true;
}

void Ground::draw(float minX, float maxX, float minY, float maxY) {
    auto editor = getDrawGridLayer()->m_editorLayer;
    if (editor->m_playbackMode == PlaybackMode::Not) return;
    
    auto& gamestate = editor->m_gameState;
    
    if (gamestate.m_unkBool8) return;

    if (!editor->m_player1->isInBasicMode() || gamestate.m_isDualMode) {
        float minPortalY = getMinPortalY();
        float maxPortalY = getMaxPortalY();

        drawLine({minX, minPortalY}, {maxX, minPortalY}, m_impl->m_bottomGroundColor, m_impl->m_bottomGroundLineWidth);
        drawLine({minX, maxPortalY}, {maxX, maxPortalY}, m_impl->m_topGroundColor, m_impl->m_topGroundLineWidth);
    }
}

void Ground::setTopGroundColor(const GradientColor& color) {
    m_impl->m_topGroundColor = color;
}

void Ground::setBottomGroundColor(const GradientColor& color) {
    m_impl->m_bottomGroundColor = color;
}

const GradientColor& Ground::getTopGroundColor() const {
    return m_impl->m_topGroundColor;
}

const GradientColor& Ground::getBottomGroundColor() const {
    return m_impl->m_bottomGroundColor;
}

void Ground::setTopGroundLineWidth(float width) {
    m_impl->m_topGroundLineWidth = width;
}

void Ground::setBottomGroundLineWidth(float width) {
    m_impl->m_bottomGroundLineWidth = width;
}

float Ground::getTopGroundLineWidth() const {
    return m_impl->m_topGroundLineWidth;
}

float Ground::getBottomGroundLineWidth() const {
    return m_impl->m_bottomGroundLineWidth;
}

float Ground::getMinPortalY() {
    auto editor = getDrawGridLayer()->m_editorLayer;

    if (editor->m_playbackMode == PlaybackMode::Playing) {
        m_impl->m_minPortalY = editor->getMinPortalY();
    }

    return m_impl->m_minPortalY;
}

float Ground::getMaxPortalY() {
    auto editor = getDrawGridLayer()->m_editorLayer;

    if (editor->m_playbackMode == PlaybackMode::Playing) {
        m_impl->m_maxPortalY = editor->getMaxPortalY();
    }

    return m_impl->m_maxPortalY;
}