#include "../../include/DrawLayers/PositionLines.hpp"
#include "../Utils.hpp"

namespace alpha::grid {

class PositionLines::Impl final {
public:
    Color m_verticalLineColor = {0, 0, 0, 50};
    Color m_horizontalLineColor = {0, 0, 0, 50};

    float m_verticalLineWidth = 2.f;
    float m_horizontalLineWidth = 2.f;
};

PositionLines::PositionLines() : m_impl(std::make_unique<Impl>()) {}

PositionLines::~PositionLines() {}

PositionLines* PositionLines::create() {
    auto ret = new PositionLines();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool PositionLines::init() {
    if (!CCNode::init()) return false;
    setID("position-lines"_spr);
    return true;
}

void PositionLines::draw(const cocos2d::CCRect& visibleRect) {
    auto editorLayer = getDrawGridLayer()->m_editorLayer;

    if (editorLayer->m_playbackMode == PlaybackMode::Playing) return;

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

void PositionLines::setVerticalLineColor(const Color& color) {
    m_impl->m_verticalLineColor = color;
}

void PositionLines::setHorizontalLineColor(const Color& color) {
    m_impl->m_horizontalLineColor = color;
}

const Color& PositionLines::getVerticalLineColor() const {
    return m_impl->m_verticalLineColor;
}

const Color& PositionLines::getHorizontalLineColor() const {
    return m_impl->m_horizontalLineColor;
}

void PositionLines::setVerticalLineWidth(float width) {
    m_impl->m_verticalLineWidth = width;
}

void PositionLines::setHorizontalLineWidth(float width) {
    m_impl->m_horizontalLineWidth = width;
}

float PositionLines::getVerticalLineWidth() const {
    return m_impl->m_verticalLineWidth;
}

float PositionLines::getHorizontalLineWidth() const {
    return m_impl->m_horizontalLineWidth;
}

const Color& PositionLines::getDefaultVerticalLineColor() {
    static Color defaultColor = {0, 0, 0, 50};
    return defaultColor;
}

const Color& PositionLines::getDefaultHorizontalLineColor() {
    static Color defaultColor = {0, 0, 0, 50};
    return defaultColor;
}

float PositionLines::getDefaultVerticalLineWidth() {
    return 2.f;
}

float PositionLines::getDefaultHorizontalLineWidth() {
    return 2.f;
}

}