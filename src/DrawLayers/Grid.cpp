#include "../../include/DrawLayers/Grid.hpp"

namespace alpha::grid {

class Grid::Impl final {
public:
    Color m_gridColor = {0, 0, 0, 150};
    float m_lineWidth = 1.f;
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

void Grid::draw(const cocos2d::CCRect& visibleRect) {
    auto editorLayer = getDrawGridLayer()->m_editorLayer;

    if (!editorLayer->m_showGrid || (editorLayer->m_hideGridOnPlay && editorLayer->m_playbackMode == PlaybackMode::Playing)) return;
    
    auto gridSize = getDrawGridLayer()->m_gridSize;
    auto size = getGridBoundsSize();
    auto origin = getGridBoundsOrigin();

    float scale = editorLayer->m_objectLayer->getScale();
    float xStart = std::max(visibleRect.getMinX() - gridSize, origin.x);
    float xEnd = std::min(visibleRect.getMaxX() + gridSize, size.width);
    
    float invGridSize = 1.f / gridSize;

    int firstGridX = static_cast<int>(std::floor(xStart  * invGridSize));
    int lastGridX = static_cast<int>(std::floor(xEnd * invGridSize)) - 1;
    
    float yStart = std::max(visibleRect.getMinY() - gridSize, origin.y);
    float yEnd = std::min(visibleRect.getMaxY() + gridSize, (editorLayer->m_levelSettings->m_dynamicLevelHeight ? size.height : DrawGridBase::MaxHeight));
    
    int firstGridY = static_cast<int>(std::floor(yStart * invGridSize));
    int lastGridY = static_cast<int>(std::floor(yEnd * invGridSize)) - 1;
    
    float x = firstGridX * gridSize + gridSize;
    for (int i = firstGridX; i <= lastGridX; i++, x += gridSize) {
        drawLine({x, visibleRect.getMinY()}, {x, visibleRect.getMaxY()}, m_impl->m_gridColor, m_impl->m_lineWidth);
    }

    float y = firstGridY * gridSize + gridSize;
    for (int i = firstGridY; i <= lastGridY; i++, y += gridSize) {
        drawLine({visibleRect.getMinX(), y}, {visibleRect.getMaxX(), y}, m_impl->m_gridColor, m_impl->m_lineWidth);
    }
}

void Grid::setGridColor(const Color& color) {
    m_impl->m_gridColor = color;
}

const Color& Grid::getGridColor() const {
    return m_impl->m_gridColor;
}

void Grid::setLineWidth(float width) {
    m_impl->m_lineWidth = width;
}

float Grid::getLineWidth() const {
    return m_impl->m_lineWidth;
}

}