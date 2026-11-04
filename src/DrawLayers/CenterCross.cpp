#include "../../include/DrawLayers/CenterCross.hpp"
#include "../Utils.hpp"

namespace alpha::grid {

class CenterCross::Impl final {
public:
    Color m_verticalLineColor = {0, 0, 0, 50};
    Color m_horizontalLineColor = {0, 0, 0, 50};

    float m_verticalLineWidth = 2.f;
    float m_horizontalLineWidth = 2.f;
};

CenterCross::CenterCross() : m_impl(std::make_unique<Impl>()) {}

CenterCross::~CenterCross() {}

CenterCross* CenterCross::create() {
    auto ret = new CenterCross();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CenterCross::init() {
    if (!CCNode::init()) return false;
    setID("center-cross"_spr);
    return true;
}

void CenterCross::draw(const cocos2d::CCRect& visibleRect) {
    auto editorLayer = getDrawGridLayer()->m_editorLayer;

    if (editorLayer->m_playbackMode == PlaybackMode::Playing || editorLayer->m_hideCenterCross || editorLayer->m_editorUI->m_uiHidden) return;

    auto winSize = getDrawGridLayer()->getContentSize();
    auto toolbarHeight = editorLayer->m_editorUI->m_toolbarHeight;

    auto objectLayer = editorLayer->m_objectLayer;

    auto screenCenter = winSize / 2.f;
    auto pivotInObject = objectLayer->convertToNodeSpace(screenCenter);
    auto lineScreenPos = winSize / 2.f + CCSize{0.f, toolbarHeight / 2.f};
    auto linePosInObject = objectLayer->convertToNodeSpace(lineScreenPos);

    float dx = linePosInObject.x - pivotInObject.x;
    float dy = linePosInObject.y - pivotInObject.y;

    auto custom = alpha::grid::utils::getDrawGridLayer()->getCustom();

    float rotatedX = custom->getCos() * dx + custom->getSin() * dy + pivotInObject.x;
    float rotatedY = -custom->getSin() * dx + custom->getCos() * dy + pivotInObject.y;

    if (rotatedX >= visibleRect.getMinX() && rotatedX <= visibleRect.getMaxX()) {
        drawLine({rotatedX, visibleRect.getMinY()}, {rotatedX, visibleRect.getMaxY()}, m_impl->m_verticalLineColor, m_impl->m_verticalLineWidth);
    }

    if (rotatedY >= visibleRect.getMinY() && rotatedY <= visibleRect.getMaxY()) {
        drawLine({visibleRect.getMinX(), rotatedY}, {visibleRect.getMaxX(), rotatedY}, m_impl->m_horizontalLineColor, m_impl->m_horizontalLineWidth);
    }
}

void CenterCross::setVerticalLineColor(const Color& color) {
    m_impl->m_verticalLineColor = color;
}

void CenterCross::setHorizontalLineColor(const Color& color) {
    m_impl->m_horizontalLineColor = color;
}

const Color& CenterCross::getVerticalLineColor() const {
    return m_impl->m_verticalLineColor;
}

const Color& CenterCross::getHorizontalLineColor() const {
    return m_impl->m_horizontalLineColor;
}

void CenterCross::setVerticalLineWidth(float width) {
    m_impl->m_verticalLineWidth = width;
}

void CenterCross::setHorizontalLineWidth(float width) {
    m_impl->m_horizontalLineWidth = width;
}

float CenterCross::getVerticalLineWidth() const {
    return m_impl->m_verticalLineWidth;
}

float CenterCross::getHorizontalLineWidth() const {
    return m_impl->m_horizontalLineWidth;
}

const Color& CenterCross::getDefaultVerticalLineColor() {
    static Color defaultColor = {0, 0, 0, 50};
    return defaultColor;
}

const Color& CenterCross::getDefaultHorizontalLineColor() {
    static Color defaultColor = {0, 0, 0, 50};
    return defaultColor;
}

float CenterCross::getDefaultVerticalLineWidth() {
    return 2.f;
}

float CenterCross::getDefaultHorizontalLineWidth() {
    return 2.f;
}

}