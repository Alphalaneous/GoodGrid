#include "../../include/DrawLayers/Grid.hpp"
#include "../DrawGridLayer.hpp"

using namespace good_grid;

class Grid::Impl final {
public:
    GradientColor m_gridColor = { 0, 0, 0, 150 };
    float m_lineWidth = 1.0f;
    bool m_invert = false;
};

Grid::Grid() : m_impl(std::make_unique<Impl>()) {}

Grid::~Grid() {}

Grid* Grid::create() {
    auto ret = new Grid();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Grid::init() {
    if (!CCNode::init()) return false;
    setID("grid"_spr);
    return true;
}

void Grid::draw(float minX, float maxX, float minY, float maxY) {
    auto editorLayer = getDrawGridLayer()->m_editorLayer;

    if (!editorLayer->m_showGrid || (editorLayer->m_hideGridOnPlay && editorLayer->m_playbackMode == PlaybackMode::Playing)) return;
    
    auto& gridSize = getDrawGridLayer()->m_gridSize;
    
    const CCSize size = getGridBoundsSize();
    const CCPoint origin = getGridBoundsOrigin();

    const float scale = editorLayer->m_objectLayer->getScale();
    const float xStart = std::max(minX - gridSize, origin.x);
    const float xEnd   = std::min(maxX + gridSize, size.width);
    
    const float invGridSize = 1.0f / gridSize;

    const int firstGridX = static_cast<int>(std::floor(xStart  * invGridSize));
    const int lastGridX  = static_cast<int>(std::floor(xEnd * invGridSize)) - 1;
    
    const float yStart = std::max(minY - gridSize, origin.y);
    const float yEnd   = std::min(maxY + gridSize, (editorLayer->m_levelSettings->m_dynamicLevelHeight ? size.height : DrawHandler::MAX_HEIGHT));
    
    const int firstGridY = static_cast<int>(std::floor(yStart * invGridSize));
    const int lastGridY  = static_cast<int>(std::floor(yEnd * invGridSize)) - 1;
    
    float x = firstGridX * gridSize + gridSize;

    for (int i = firstGridX; i <= lastGridX; ++i, x += gridSize) {
        drawLine({x, minY}, {x, maxY}, m_impl->m_gridColor, m_impl->m_lineWidth, m_impl->m_invert ? BlendMode::INVERT : BlendMode::ADDITIVE);
    }

    float y = firstGridY * gridSize + gridSize;
    for (int i = firstGridY; i <= lastGridY; ++i, y += gridSize) {
        drawLine({minX, y}, {maxX, y}, m_impl->m_gridColor, m_impl->m_lineWidth, m_impl->m_invert ? BlendMode::INVERT : BlendMode::ADDITIVE);
    }
}

void Grid::setGridColor(const GradientColor& color) {
    m_impl->m_gridColor = color;
}

const GradientColor& Grid::getGridColor() const {
    return m_impl->m_gridColor;
}

void Grid::setLineWidth(float width) {
    m_impl->m_lineWidth = width;
}

float Grid::getLineWidth() const {
    return m_impl->m_lineWidth;
}

void Grid::setInverted(bool inverted) {
    m_impl->m_invert = inverted;
}

bool Grid::isInverted() {
    return m_impl->m_invert;
}