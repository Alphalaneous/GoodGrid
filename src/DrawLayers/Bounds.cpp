#include "../../include/DrawLayers/Bounds.hpp"
#include "../DrawGridLayer.hpp"

using namespace good_grid;

class Bounds::Impl final {
public:
    ccColor4B m_topBoundColor = { 255, 255, 255, 255 };
    ccColor4B m_bottomBoundColor = { 255, 255, 255, 255 };
    ccColor4B m_verticalBoundColor = { 255, 255, 255, 255 };

    float m_topBoundLineWidth = 2.0f;
    float m_bottomBoundLineWidth = 2.0f;
    float m_verticalBoundLineWidth = 1.0f;
};

Bounds::Bounds() : m_impl(std::make_unique<Impl>()) {}

Bounds::~Bounds() {}

Bounds* Bounds::create() {
    auto ret = new Bounds();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Bounds::init() {
    if (!CCNode::init()) return false;
    setID("bounds"_spr);
    return true;
}

void Bounds::draw(float minX, float maxX, float minY, float maxY) {
    drawLine({0, minY}, {0, maxY}, m_impl->m_verticalBoundColor, m_impl->m_verticalBoundLineWidth);
    if (!getDrawGridLayer()->m_editorLayer->m_showGround) {
        drawLine({minX, DrawHandler::GROUND_OFFSET}, {maxX, DrawHandler::GROUND_OFFSET}, m_impl->m_bottomBoundColor, m_impl->m_bottomBoundLineWidth);
    }
    drawLine({minX, maxY}, {maxX, maxY}, m_impl->m_topBoundColor, m_impl->m_topBoundLineWidth);
}

void Bounds::setTopBoundColor(const ccColor4B& color) {
    m_impl->m_topBoundColor = color;
}

void Bounds::setBottomBoundColor(const ccColor4B& color) {
    m_impl->m_bottomBoundColor = color;
}

void Bounds::setVerticalBoundColor(const ccColor4B& color) {
    m_impl->m_verticalBoundColor = color;
}

const ccColor4B& Bounds::getTopBoundColor() const {
    return m_impl->m_topBoundColor;
}

const ccColor4B& Bounds::getBottomBoundColor() const {
    return m_impl->m_bottomBoundColor;
}

const ccColor4B& Bounds::getVerticalColor() const {
    return m_impl->m_verticalBoundColor;
}

void Bounds::setTopBoundLineWidth(float width) {
    m_impl->m_topBoundLineWidth = width;
}

void Bounds::setBottomBoundLineWidth(float width) {
    m_impl->m_bottomBoundLineWidth = width;
}

void Bounds::setVerticalBoundLineWidth(float width) {
    m_impl->m_verticalBoundLineWidth = width;
}

float Bounds::getTopBoundLineWidth() const {
    return m_impl->m_topBoundLineWidth;
}

float Bounds::getBottomBoundLineWidth() const {
    return m_impl->m_bottomBoundLineWidth;
}

float Bounds::getVerticalLineWidth() const {
    return m_impl->m_verticalBoundLineWidth;
}
