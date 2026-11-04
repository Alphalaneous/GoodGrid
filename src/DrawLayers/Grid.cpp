#include "../../include/DrawLayers/Grid.hpp"

namespace alpha::grid {

class Grid::Impl final {
public:
    Color m_gridColor = {0, 0, 0, 150};
    float m_lineWidth = 1.f;
    std::optional<cocos2d::CCSize> m_overrideSize;
    std::optional<cocos2d::CCPoint> m_offset;
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

    float gridSizeX = gridSize;
    float gridSizeY = gridSize;

    if (m_impl->m_overrideSize) {
        gridSizeX = m_impl->m_overrideSize.value().width;
        gridSizeY = m_impl->m_overrideSize.value().height;
    }

    auto size = getGridBoundsSize();
    auto origin = getGridBoundsOrigin();

    float xStart = std::max(visibleRect.getMinX() - gridSizeX, origin.x);
    float xEnd = std::min(visibleRect.getMaxX() + gridSizeX, size.width);
    
    float yStart = std::max(visibleRect.getMinY() - gridSizeY, origin.y);
    float yEnd = std::min(visibleRect.getMaxY() + gridSizeY, (editorLayer->m_levelSettings->m_dynamicLevelHeight ? size.height : DrawGridBase::MaxHeight));

    float offsetX = 0.f;
    float offsetY = 0.f;

    if (m_impl->m_offset) {
        offsetX = std::fmod(std::fmod(m_impl->m_offset->x, gridSizeX) + gridSizeX, gridSizeX);
        offsetY = std::fmod(std::fmod(m_impl->m_offset->y, gridSizeY) + gridSizeY, gridSizeY);

        if (offsetX != 0) {
            xEnd += gridSizeX;
        }
        if (offsetY != 0) {
            yEnd += gridSizeY;
        }
    }

    float firstX = std::floor((xStart - offsetX) / gridSizeX) * gridSizeX + offsetX;
    float firstY = std::floor((yStart - offsetY) / gridSizeY) * gridSizeY + offsetY;

    for (float x = firstX; x <= xEnd; x += gridSizeX) {
        auto newX = std::clamp(x, origin.x, size.width);
        drawLine({newX, visibleRect.getMinY()}, {newX, visibleRect.getMaxY()}, m_impl->m_gridColor, m_impl->m_lineWidth);
    }

    for (float y = firstY; y <= yEnd; y += gridSizeY) {
        auto newY = std::clamp(y, origin.y, size.height);
        drawLine({visibleRect.getMinX(), newY}, {visibleRect.getMaxX(), newY}, m_impl->m_gridColor, m_impl->m_lineWidth);
    }
}

void Grid::setGridColor(const Color& color) {
    m_impl->m_gridColor = color;
}

const Color& Grid::getGridColor() const {
    return m_impl->m_gridColor;
}

const Color& Grid::getDefaultGridColor() {
    static Color defaultColor = {0, 0, 0, 150};
    return defaultColor;
}

void Grid::setLineWidth(float width) {
    m_impl->m_lineWidth = width;
}

float Grid::getLineWidth() const {
    return m_impl->m_lineWidth;
}

float Grid::getDefaultLineWidth() {
    return 1.f;
}

void Grid::overrideGridSize(std::optional<cocos2d::CCSize> size) {
    m_impl->m_overrideSize = size;
}

std::optional<cocos2d::CCSize> Grid::getGridSize() {
    return m_impl->m_overrideSize;
}

void Grid::offsetGrid(std::optional<cocos2d::CCPoint> offset) {
    m_impl->m_offset = offset;
}

std::optional<cocos2d::CCPoint> Grid::getGridOffset() {
    return m_impl->m_offset;
}

}