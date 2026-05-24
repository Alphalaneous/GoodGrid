#include "../../include/DrawLayers/PositionLines.hpp"

using namespace good_grid;

class PositionLines::Impl final {
public:
    GradientColor m_verticalLineColor = { 0, 0, 0, 50 };
    GradientColor m_horizontalLineColor = { 0, 0, 0, 50 };

    float m_verticalLineWidth = 2.0f;
    float m_horizontalLineWidth = 2.0f;

    float m_cachedAngle = -1;
    float m_cachedAngleRad = 0;
    float m_cachedSin = 0;
    float m_cachedCos = 0;
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

    if (editorLayer->m_playbackMode == PlaybackMode::Playing)
        return;

    if (!posLinesEnabledBE()) return;

    const cocos2d::CCSize& winSize = getDrawGridLayer()->getContentSize();
    const float& toolbarHeight = editorLayer->m_editorUI->m_toolbarHeight;

    auto objectLayer = editorLayer->m_objectLayer;

    cocos2d::CCPoint screenCenter = { winSize.width * 0.5f, winSize.height * 0.5f };
    cocos2d::CCPoint pivotInObject = objectLayer->convertToNodeSpace(screenCenter);
    cocos2d::CCPoint lineScreenPos = { winSize.width * 0.5f, (winSize.height + toolbarHeight) * 0.5f };
    cocos2d::CCPoint linePosInObject = objectLayer->convertToNodeSpace(lineScreenPos);

    const auto& cameraAngle = editorLayer->m_gameState.m_cameraAngle;

    if (m_impl->m_cachedAngle != cameraAngle) {
        m_impl->m_cachedAngle = cameraAngle;
        m_impl->m_cachedAngleRad = -CC_DEGREES_TO_RADIANS(cameraAngle);
        m_impl->m_cachedSin = std::sin(m_impl->m_cachedAngleRad);
        m_impl->m_cachedCos = std::cos(m_impl->m_cachedAngleRad);
    }

    float dx = linePosInObject.x - pivotInObject.x;
    float dy = linePosInObject.y - pivotInObject.y;

    float rotatedX = m_impl->m_cachedCos * dx - m_impl->m_cachedSin * dy + pivotInObject.x;
    float rotatedY = m_impl->m_cachedSin * dx + m_impl->m_cachedCos * dy + pivotInObject.y;

    if (rotatedX >= minX && rotatedX <= maxX) {
        drawLine({rotatedX, minY}, {rotatedX, maxY}, m_impl->m_verticalLineColor, m_impl->m_verticalLineWidth);
    }

    if (rotatedY >= minY && rotatedY <= maxY) {
        drawLine({minX, rotatedY}, {maxX, rotatedY}, m_impl->m_horizontalLineColor, m_impl->m_horizontalLineWidth);
    }
}

bool PositionLines::posLinesEnabledBE() {
    static auto betterEdit = geode::Loader::get()->getLoadedMod("hjfod.betteredit");
    if (!betterEdit) return true;

    return betterEdit->getSavedValue<bool>("pos-line");
}

void PositionLines::setVerticalLineColor(const GradientColor& color) {
    m_impl->m_verticalLineColor = color;
}

void PositionLines::setHorizontalLineColor(const GradientColor& color) {
    m_impl->m_horizontalLineColor = color;
}

const GradientColor& PositionLines::getVerticalLineColor() const {
    return m_impl->m_verticalLineColor;
}

const GradientColor& PositionLines::getHorizontalLineColor() const {
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
