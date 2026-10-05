#include "../../include/DrawLayers/Ground.hpp"

namespace alpha::grid {

class Ground::Impl final {
public:
    Color m_topGroundColor = {255, 175, 50, 255};
    Color m_bottomGroundColor = {255, 175, 50, 255};

    float m_topGroundLineWidth = 2.f;
    float m_bottomGroundLineWidth = 2.f;

    float m_minPortalY = 0.f;
    float m_maxPortalY = 0.f;
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

void Ground::draw(const cocos2d::CCRect& visibleRect) {
    auto editor = getDrawGridLayer()->m_editorLayer;
    if (editor->m_playbackMode == PlaybackMode::Not || editor->m_gameState.m_isFreeMode ||
        editor->m_player1->isInBasicMode()  || !editor->m_gameState.m_isDualMode) {
        return;
    }
 
    float minPortalY = getMinPortalY();
    float maxPortalY = getMaxPortalY();

    drawLine({visibleRect.getMinX(), minPortalY}, {visibleRect.getMaxX(), minPortalY}, m_impl->m_bottomGroundColor, m_impl->m_bottomGroundLineWidth);
    drawLine({visibleRect.getMinX(), maxPortalY}, {visibleRect.getMaxX(), maxPortalY}, m_impl->m_topGroundColor, m_impl->m_topGroundLineWidth);
}

void Ground::setTopGroundColor(const Color& color) {
    m_impl->m_topGroundColor = color;
}

void Ground::setBottomGroundColor(const Color& color) {
    m_impl->m_bottomGroundColor = color;
}

const Color& Ground::getTopGroundColor() const {
    return m_impl->m_topGroundColor;
}

const Color& Ground::getBottomGroundColor() const {
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

const Color& Ground::getDefaultTopGroundColor() {
    static Color defaultColor = {255, 175, 50, 255};
    return defaultColor;
}

const Color& Ground::getDefaultBottomGroundColor() {
    static Color defaultColor = {255, 175, 50, 255};
    return defaultColor;
}

float Ground::getDefaultTopGroundLineWidth() {
    return 2.f;
}

float Ground::getDefaultBottomGroundLineWidth() {
    return 2.f;
}

}