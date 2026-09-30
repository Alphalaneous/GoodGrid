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

void PositionLines::draw(float minX, float maxX, float minY, float maxY) {
    auto editorLayer = getDrawGridLayer()->m_editorLayer;

    if (editorLayer->m_playbackMode == PlaybackMode::Playing) return;

    auto winSize = getDrawGridLayer()->getContentSize();
    auto toolbarHeight = editorLayer->m_editorUI->m_toolbarHeight;

    auto objectLayer = editorLayer->m_objectLayer;

    auto screenCenter = cocos2d::CCPoint{winSize.width * 0.5f, winSize.height * 0.5f};
    auto pivotInObject = objectLayer->convertToNodeSpace(screenCenter);
    auto lineScreenPos = cocos2d::CCPoint{winSize.width * 0.5f, (winSize.height + toolbarHeight) * 0.5f};
    auto linePosInObject = objectLayer->convertToNodeSpace(lineScreenPos);

    float dx = linePosInObject.x - pivotInObject.x;
    float dy = linePosInObject.y - pivotInObject.y;

    auto custom = alpha::grid::utils::getDrawGridLayer()->getCustom();

    float rotatedX = custom->getCos() * dx - custom->getSin() * dy + pivotInObject.x;
    float rotatedY = custom->getSin() * dx + custom->getCos() * dy + pivotInObject.y;

    if (rotatedX >= minX && rotatedX <= maxX) {
        drawLine({rotatedX, minY}, {rotatedX, maxY}, m_impl->m_verticalLineColor, m_impl->m_verticalLineWidth);
    }

    if (rotatedY >= minY && rotatedY <= maxY) {
        drawLine({minX, rotatedY}, {maxX, rotatedY}, m_impl->m_horizontalLineColor, m_impl->m_horizontalLineWidth);
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

}