#include "../include/DrawLayers.hpp"
#include "../include/DrawGridAPI.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

void Grid::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    auto editorLayer = dgl->m_editorLayer;

    if (!editorLayer->m_showGrid || (editorLayer->m_hideGridOnPlay && editorLayer->m_playbackMode == PlaybackMode::Playing)) return;
    
    auto& api = DrawGridAPI::get();
    auto& gridSize = dgl->m_gridSize;
    
    const CCSize size = api.getGridBoundsSize();
    const CCPoint origin = api.getGridBoundsOrigin();

    const float scale = editorLayer->m_objectLayer->getScale();
    const float xStart = std::max(minX - gridSize, origin.x);
    const float xEnd   = std::min(maxX + gridSize, size.width);
    
    const float invGridSize = 1.0f / gridSize;

    const int firstGridX = static_cast<int>(std::floor(xStart  * invGridSize));
    const int lastGridX  = static_cast<int>(std::ceil(xEnd * invGridSize));
    
    const float yStart = std::max(minY - gridSize, origin.y);
    const float yEnd   = std::min(maxY + gridSize, (editorLayer->m_levelSettings->m_dynamicLevelHeight ? size.height : MAX_HEIGHT));
    
    const int firstGridY = static_cast<int>(std::floor(yStart * invGridSize));
    const int lastGridY  = static_cast<int>(std::ceil(yEnd * invGridSize));
    
    float x = firstGridX * gridSize + gridSize;
    for (int i = firstGridX; i <= lastGridX; ++i, x += gridSize) {
        api.drawLine({x, minY}, {x, maxY}, m_gridColor, m_lineWidth);
    }

    float y = firstGridY * gridSize + gridSize;
    for (int i = firstGridY; i <= lastGridY; ++i, y += gridSize) {
        api.drawLine({minX, y}, {maxX, y}, m_gridColor, m_lineWidth);
    }
}

void Grid::setGridColor(const LineColor& color, int priority) {
    if (priority >= m_gridColorPriority) {
        m_gridColor = color;
        m_gridColorPriority = priority;
    }
}

int Grid::getGridColorPriority() const { 
    return m_gridColorPriority; 
}

const LineColor& Grid::getGridColor() const { 
    return m_gridColor; 
}

void Grid::setLineWidth(float width, int priority) {
    if (priority >= m_gridColorPriority) {
        m_lineWidth = width;
        m_lineWidthPriority = priority;
    }
}

int Grid::getLineWidthPriority() const {
    return m_lineWidthPriority;
}

float Grid::getLineWidth() const {
    return m_lineWidth;
}

void Bounds::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    auto& api = DrawGridAPI::get();

    api.drawLine({0, minY}, {0, maxY}, m_verticalBoundColor, m_verticalBoundLineWidth);
    if (!dgl->m_editorLayer->m_showGround) {
        api.drawLine({minX, GROUND_OFFSET}, {maxX, GROUND_OFFSET}, m_bottomBoundColor, m_bottomBoundLineWidth);
    }
    api.drawLine({minX, maxY}, {maxX, maxY}, m_topBoundColor, m_topBoundLineWidth);
}

void Bounds::setTopBoundColor(const LineColor& color, int priority) {
    if (priority >= m_topBoundColorPriority) {
        m_topBoundColor = color;
        m_topBoundColorPriority = priority;
    }
}

void Bounds::setBottomBoundColor(const LineColor& color, int priority) {
    if (priority >= m_bottomBoundColorPriority) {
        m_bottomBoundColor = color;
        m_bottomBoundColorPriority = priority;
    }
}

void Bounds::setVerticalBoundColor(const LineColor& color, int priority) {
    if (priority >= m_verticalBoundColorPriority) {
        m_verticalBoundColor = color;
        m_verticalBoundColorPriority = priority;
    }
}

int Bounds::getTopBoundColorPriority() const {
    return m_topBoundColorPriority;
}

int Bounds::getBottomBoundColorPriority() const {
    return m_bottomBoundColorPriority;
}

int Bounds::getVerticalBoundColorPriority() const {
    return m_verticalBoundColorPriority;
}

const LineColor& Bounds::getTopBoundColor() const {
    return m_topBoundColor;
}

const LineColor& Bounds::getBottomBoundColor() const {
    return m_bottomBoundColor;
}

const LineColor& Bounds::getVerticalColor() const {
    return m_verticalBoundColor;
}

void Bounds::setTopBoundLineWidth(float width, int priority) {
    if (priority >= m_topBoundLineWidthPriority) {
        m_topBoundLineWidth = width;
        m_topBoundLineWidthPriority = priority;
    }
}

void Bounds::setBottomBoundLineWidth(float width, int priority) {
    if (priority >= m_bottomBoundLineWidthPriority) {
        m_bottomBoundLineWidth = width;
        m_bottomBoundLineWidthPriority = priority;
    }
}

void Bounds::setVerticalBoundLineWidth(float width, int priority) {
    if (priority >= m_verticalBoundLineWidthPriority) {
        m_verticalBoundLineWidth = width;
        m_verticalBoundLineWidthPriority = priority;
    }
}

int Bounds::getTopBoundLineWidthPriority() const {
    return m_topBoundLineWidthPriority;
}

int Bounds::getBottomBoundLineWidthPriority() const {
    return m_bottomBoundLineWidthPriority;
}

int Bounds::getVerticalBoundLineWidthPriority() const {
    return m_verticalBoundLineWidthPriority;
}

float Bounds::getTopBoundLineWidth() const {
    return m_topBoundLineWidth;
}

float Bounds::getBottomBoundLineWidth() const {
    return m_bottomBoundLineWidth;
}

float Bounds::getVerticalLineWidth() const {
    return m_verticalBoundLineWidth;
}

void Ground::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    auto editorLayer = dgl->m_editorLayer;
    if (editorLayer->m_playbackMode != PlaybackMode::Playing) return;
    auto& gamestate = editorLayer->m_gameState;
    auto& api = DrawGridAPI::get();
    
    if (gamestate.m_unkBool8) return;

    if (!editorLayer->m_player1->isInBasicMode() || gamestate.m_isDualMode) {
        float minPortalY = editorLayer->getMinPortalY();
        float maxPortalY = editorLayer->getMaxPortalY();

        api.drawLine({minX, minPortalY}, {maxX, minPortalY}, m_bottomGroundColor, m_bottomGroundLineWidth);
        api.drawLine({minX, maxPortalY}, {maxX, maxPortalY}, m_topGroundColor, m_topGroundLineWidth);
    }
}

void Ground::setTopGroundColor(const LineColor& color, int priority) {
    if (priority >= m_topGroundColorPriority) {
        m_topGroundColor = color;
        m_topGroundColorPriority = priority;
    }
}

void Ground::setBottomGroundColor(const LineColor& color, int priority) {
    if (priority >= m_bottomGroundColorPriority) {
        m_bottomGroundColor = color;
        m_bottomGroundColorPriority = priority;
    }
}

int Ground::getTopGroundColorPriority() const {
    return m_topGroundColorPriority;
}

int Ground::getBottomGroundColorPriority() const {
    return m_bottomGroundColorPriority;
}

const LineColor& Ground::getTopGroundColor() const {
    return m_topGroundColor;
}

const LineColor& Ground::getBottomGroundColor() const {
    return m_bottomGroundColor;
}

void Ground::setTopGroundLineWidth(float width, int priority) {
    if (priority >= m_topGroundLineWidthPriority) {
        m_topGroundLineWidth = width;
        m_topGroundLineWidthPriority = priority;
    }
}

void Ground::setBottomGroundLineWidth(float width, int priority) {
    if (priority >= m_bottomGroundLineWidthPriority) {
        m_bottomGroundLineWidth = width;
        m_bottomGroundLineWidthPriority = priority;
    }
}

int Ground::getTopGroundLineWidthPriority() const {
    return m_topGroundLineWidthPriority;
}

int Ground::getBottomGroundLineWidthPriority() const {
    return m_bottomGroundColorPriority;
}

float Ground::getTopGroundLineWidth() const {
    return m_topGroundLineWidth;
}

float Ground::getBottomGroundLineWidth() const {
    return m_bottomGroundLineWidth;
}

void GuideObjects::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    if (dgl->m_editorLayer->m_playbackMode == PlaybackMode::Playing) return;
    auto& api = DrawGridAPI::get();

    m_colorsForObject.rebuildIfNeeded();

    for (auto obj : CCArrayExt<EffectGameObject*>(dgl->m_guideObjects)) {
        if (!api.isObjectVisible(obj)) continue;
        auto [y1, y2] = api.getPortalMinMax(obj);

        static const auto defaultLineColor = LineColor{0, 255, 255, 255};

        LineColor bottomColor = defaultLineColor;
        LineColor topColor = defaultLineColor;

        float lineWidthBottom = 2.0f;
        float lineWidthTop = 2.0f;

        for (auto& fn : m_colorsForObject.flat) {
            fn(bottomColor, topColor, obj, lineWidthBottom, lineWidthTop);
        }

        if (y1 >= minY && y1 <= maxY) api.drawLine({minX, y1}, {maxX, y1}, bottomColor, lineWidthBottom);
        if (y2 >= minY && y2 <= maxY) api.drawLine({minX, y2}, {maxX, y2}, topColor, lineWidthTop);
    }
}

void GuideObjects::setPropertiesForObject(GuideObjectCallback colorForObject, int priority) {
    m_colorsForObject.add(std::move(colorForObject), priority);
}

void EffectLines::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    auto editorLayer = dgl->m_editorLayer;

    if (!editorLayer->m_drawEffectLines || editorLayer->m_playbackMode == PlaybackMode::Playing) return;
    auto& api = DrawGridAPI::get();

    m_colorsForObject.rebuildIfNeeded();

    for (auto obj : CCArrayExt<EffectGameObject*>(dgl->m_effectGameObjects)) {
        if (obj->m_isSpawnTriggered || obj->m_isTouchTriggered || !api.isObjectVisible(obj)) continue;
        float x = obj->getPositionX();
        if (x < minX || x > maxX || x < 0) continue;

        static const auto defaultLineColor = LineColor{0, 255, 255, 255};

        LineColor color = defaultLineColor;

        float lineWidth = 1.0f;

        for (auto& fn : m_colorsForObject.flat) {
            fn(color, x, obj, lineWidth);
        }

        api.drawLine({x, minY}, { x, maxY}, color, lineWidth);
    }
}

void EffectLines::setPropertiesForObject(EffectLineCallback colorForObject, int priority) {
    m_colorsForObject.add(std::move(colorForObject), priority);
}

void DurationLines::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    auto editorLayer = dgl->m_editorLayer;

    if (!editorLayer->m_showDurationLines || editorLayer->m_playbackMode == PlaybackMode::Playing) return;
    auto& api = DrawGridAPI::get();

    m_colorsForObject.rebuildIfNeeded();

    const auto& startSpeed = editorLayer->m_levelSettings->m_startSpeed;
    const auto& isPlatformer = editorLayer->m_isPlatformer;
    auto& rotateChannel = editorLayer->m_gameState.m_rotateChannel;
    const auto& updateTimeMarkers = dgl->m_updateTimeMarkers;
    auto speedObjects = dgl->m_speedObjects;
    auto snapObject = editorLayer->m_editorUI->m_snapObject;

    for (auto obj : CCArrayExt<EffectGameObject*>(editorLayer->m_durationObjects)) {
        if (!api.isObjectVisible(obj)) continue;
        
        static const auto defaultLineColor = LineColor{255, 255, 255, 115};

        LineColor color = defaultLineColor;

        float lineWidth = 2.0f;

        for (auto& fn : m_colorsForObject.flat) {
            fn(color, obj, lineWidth);
        }

        CCPoint& endPos = obj->m_endPosition;

        if (updateTimeMarkers || (obj == m_lastSnappedObject && !snapObject)) {
            endPos = CCPoint{0, 0};
            m_lastSnappedObject = nullptr;
        }
        else {
            m_lastSnappedObject = snapObject;
        }

        float time = obj->m_duration;

        if (obj->m_objectID == 1006) {
            time = obj->m_fadeInDuration + obj->m_holdDuration + obj->m_fadeOutDuration;
        }
        else if (obj->m_objectID == 3602) {
            SFXTriggerGameObject* sfxTrigger = static_cast<SFXTriggerGameObject*>(obj);
            time = sfxTrigger->m_soundDuration;
        }

        if ((time <= 0)) continue;
        
        const CCPoint& currentPos = obj->getPosition();

        if (!obj->m_isSpawnTriggered) {
            if (endPos == CCPointZero) {
                float currentTime = LevelTools::timeForPos(
                    currentPos,
                    speedObjects,
                    static_cast<int>(startSpeed),
                    obj->m_ordValue,
                    obj->m_channelValue,
                    false,
                    isPlatformer,
                    true,
                    false,
                    false
                );

                bool wasRotated = LevelTools::getLastGameplayRotated();

                CCPoint newPos = LevelTools::posForTimeInternal(
                    currentTime + time,
                    speedObjects,
                    static_cast<int>(startSpeed),
                    isPlatformer,
                    false,
                    true,
                    rotateChannel,
                    false
                );

                bool nowRotated = LevelTools::getLastGameplayRotated();

                if (wasRotated == nowRotated) {
                    if (wasRotated) {
                        endPos.x = currentPos.x;
                        endPos.y = newPos.y;
                    } else {
                        endPos.x = newPos.x;
                        endPos.y = currentPos.y;
                    }
                }
                else {
                    endPos = newPos;
                }
            }
        }
        else {
            endPos.x = currentPos.x + time * 311.5801f;
            endPos.y = currentPos.y;
        }

        if (endPos.x < minX || currentPos.x > maxX || endPos.y < minY || currentPos.y > maxY) continue;

        api.drawLine({currentPos.x, currentPos.y}, {endPos.x, endPos.y}, color, lineWidth, false);
    }
    dgl->m_updateTimeMarkers = false;
}

void DurationLines::setPropertiesForObject(DurationLineCallback colorForObject, int priority) {
    m_colorsForObject.add(std::move(colorForObject), priority);
}

void Guidelines::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    if (!GameManager::get()->m_showSongMarkers) return;
    auto& api = DrawGridAPI::get();

    m_colorsForValue.rebuildIfNeeded();

    for (const auto& [k, v] : api.getTimeMarkers()) {
        LineColor color = v;
        float x = k;
        float lineWidth = 1.0f;

        for (auto& fn : m_colorsForValue.flat) {
            fn(color, x, lineWidth);
        }

        if (x < minX || x > maxX) continue;
        api.drawLine({x, minY}, {x, maxY}, color, lineWidth);
    }
}

void Guidelines::setPropertiesForValue(GuidelineCallback colorForValue, int priority) {
    m_colorsForValue.add(std::move(colorForValue), priority);
}

void BPMTriggers::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    auto& api = DrawGridAPI::get();

    m_colorsForBeats.rebuildIfNeeded();

    for (auto& [_, obj] : dgl->m_audioLineObjects) {
        if (obj->m_disabled || !api.isObjectVisible(obj)) continue;
        float speed = dgl->m_normalSpeed;
        
        switch (obj->m_speed) {
            case Speed::Slow:    speed = dgl->m_slowSpeed;    break;
            case Speed::Fast:    speed = dgl->m_fastSpeed;    break;
            case Speed::Faster:  speed = dgl->m_fasterSpeed;  break;
            case Speed::Fastest: speed = dgl->m_fastestSpeed; break;
            default: break;
        }
        const float startX = obj->getPositionX();
        const float duration = obj->m_duration * speed;
        const float endX = startX + duration;
        const int beatsPerBar = obj->m_beatsPerBar;
        
        if (obj->m_beatsPerMinute == 0 || beatsPerBar == 0) continue;
        const float timeStep = speed * 60.f / (obj->m_beatsPerMinute * beatsPerBar);
       
        int beatStart = std::max(0, static_cast<int>(std::floor((minX - startX) / timeStep)));
        int beatEnd = static_cast<int>(std::ceil((maxX - startX) / timeStep));
        
        for (int beat = beatStart; beat <= beatEnd; ++beat) {
            float x = startX + timeStep * beat;

            static const auto defaultLineColorA = LineColor{255, 255, 0, 255};
            static const auto defaultLineColorB = LineColor{255, 127, 0, 255};

            LineColor color;
            float lineWidth = 1.0f;
            if (beat % beatsPerBar == 0) {
                color = defaultLineColorA;
            }
            else {
                color = defaultLineColorB;
            }

            for (auto& fn : m_colorsForBeats.flat) {
                fn(color, obj, x, beat, beatsPerBar, lineWidth);
            }

            if (x < minX || x > maxX) continue;
            if (x > endX || beat > beatEnd) break;

            api.drawLine({x, minY}, {x, maxY}, color, lineWidth);
        }
    }
}

void BPMTriggers::setPropertiesForBeats(BPMTriggerCallback colorForBeats, int priority) {
    m_colorsForBeats.add(std::move(colorForBeats), priority);
}

void AudioLine::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    LineColor color = {50, 255, 50, 255};
    auto& api = DrawGridAPI::get();
    auto editorLayer = dgl->m_editorLayer;

    m_colorsForTime.rebuildIfNeeded();

    const auto& startSpeed = editorLayer->m_levelSettings->m_startSpeed;
    const auto& isPlatformer = editorLayer->m_isPlatformer;
    const auto& playbackActive = editorLayer->m_playbackActive;
    auto& rotateChannel = editorLayer->m_gameState.m_rotateChannel;
    auto& playbackX = dgl->m_playbackX;
    auto& playbackY = dgl->m_playbackY;
    auto& playbackTime = dgl->m_playbackTime;

    auto speedObjects = dgl->m_speedObjects;

    float width = 5.0f;
    if (playbackActive) {
        CCPoint pos = LevelTools::posForTimeInternal(
            playbackTime,
            speedObjects,
            static_cast<int>(startSpeed),
            isPlatformer,
            true,
            true,
            rotateChannel,
            false
        );
        playbackX = 0;
        playbackY = 0;

        if (LevelTools::getLastGameplayRotated()) {
            playbackY = pos.y;
        } else {
            playbackX = pos.x;
        }
    } else {
        color = {2, 255, 2, 100};
        width = 3.0f;
    }

    if (editorLayer->m_playbackMode == PlaybackMode::Playing) {
        playbackX = 0;
        playbackY = 0;
        playbackTime = 0;
    }

    for (auto& fn : m_colorsForTime.flat) {
        fn(color, playbackActive, playbackTime, {playbackX, playbackY}, width);
    }

    if (playbackX != 0) {
        api.drawLine({playbackX, minY}, {playbackX, maxY}, color, width, !playbackActive);
    }
    if (playbackY != 0) {
        api.drawLine({minX, playbackY}, {maxX, playbackY}, color, width, !playbackActive);
    }
}

void AudioLine::setPropertiesForTime(AudioLineCallback colorForTime, int priority) {
    m_colorsForTime.add(std::move(colorForTime), priority);
}

void PositionLines::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    auto editorLayer = dgl->m_editorLayer;

    if (editorLayer->m_playbackMode == PlaybackMode::Playing)
        return;

    if (!posLinesEnabledBE()) return;

    auto& api = DrawGridAPI::get();

    const CCSize& winSize = dgl->getContentSize();
    const float& toolbarHeight = editorLayer->m_editorUI->m_toolbarHeight;

    auto objectLayer = editorLayer->m_objectLayer;

    CCPoint screenCenter = { winSize.width * 0.5f, winSize.height * 0.5f };
    CCPoint pivotInObject = objectLayer->convertToNodeSpace(screenCenter);
    CCPoint lineScreenPos = { winSize.width * 0.5f, (winSize.height + toolbarHeight) * 0.5f };
    CCPoint linePosInObject = objectLayer->convertToNodeSpace(lineScreenPos);

    const auto& cameraAngle = editorLayer->m_gameState.m_cameraAngle;

    if (m_cachedAngle != cameraAngle) {
        m_cachedAngle = cameraAngle;
        m_cachedAngleRad = -CC_DEGREES_TO_RADIANS(cameraAngle);
        m_cachedSin = std::sin(m_cachedAngleRad);
        m_cachedCos = std::cos(m_cachedAngleRad);
    }

    float dx = linePosInObject.x - pivotInObject.x;
    float dy = linePosInObject.y - pivotInObject.y;

    float rotatedX = m_cachedCos * dx - m_cachedSin * dy + pivotInObject.x;
    float rotatedY = m_cachedSin * dx + m_cachedCos * dy + pivotInObject.y;

    if (rotatedX >= minX && rotatedX <= maxX) {
        api.drawLine({rotatedX, minY}, {rotatedX, maxY}, m_verticalLineColor, m_verticalLineWidth);
    }

    if (rotatedY >= minY && rotatedY <= maxY) {
        api.drawLine({minX, rotatedY}, {maxX, rotatedY}, m_horizontalLineColor, m_horizontalLineWidth);
    }
}

bool PositionLines::posLinesEnabledBE() {
    static auto betterEdit = Loader::get()->getLoadedMod("hjfod.betteredit");
    if (!betterEdit) return true;

    static bool enabled = betterEdit->getSettingValue<bool>("pos-line");
    static auto listener = listenForSettingChanges("pos-line", [](bool value) {
        enabled = value;
    }, betterEdit);

    return enabled;
}

void PositionLines::setVerticalLineColor(const LineColor& color, int priority) {
    if (priority >= m_verticalLineColorPriority) {
        m_verticalLineColor = color;
        m_verticalLineColorPriority = priority;
    }
}

void PositionLines::setHorizontalLineColor(const LineColor& color, int priority) {
    if (priority >= m_horizontalLineColorPriority) {
        m_horizontalLineColor = color;
        m_horizontalLineColorPriority = priority;
    }
}

int PositionLines::getVerticalLineColorPriority() const {
    return m_verticalLineColorPriority;
}

int PositionLines::getHorizontalLineColorPriority() const {
    return m_horizontalLineColorPriority;
}

const LineColor& PositionLines::getVerticalLineColor() const {
    return m_verticalLineColor;
}

const LineColor& PositionLines::getHorizontalLineColor() const {
    return m_horizontalLineColor;
}

void PositionLines::setVerticalLineWidth(float width, int priority) {
    if (priority >= m_verticalLineWidthPriority) {
        m_verticalLineWidth = width;
        m_verticalLineWidthPriority = priority;
    }
}

void PositionLines::setHorizontalLineWidth(float width, int priority) {
    if (priority >= m_horizontalLineWidthPriority) {
        m_horizontalLineWidth = width;
        m_horizontalLineWidthPriority = priority;
    }
}

int PositionLines::getVerticalLineWidthPriority() const {
    return m_verticalLineWidthPriority;
}

int PositionLines::getHorizontalLineWidthPriority() const {
    return m_horizontalLineWidthPriority;
}

float PositionLines::getVerticalLineWidth() const {
    return m_verticalLineWidth;
}

float PositionLines::getHorizontalLineWidth() const {
    return m_horizontalLineWidth;
}

void PreviewLockLine::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    auto editorLayer = dgl->m_editorLayer;

    if (editorLayer->m_playbackMode != PlaybackMode::Not) return;
    if (editorLayer->m_previewPosition.x <= 0) return;
    const CCPoint& pos = editorLayer->m_previewPosition;
    DrawGridAPI::get().drawLine({pos.x, minY}, {pos.x, maxY}, m_lineColor, m_lineWidth);
}

void PreviewLockLine::setLineColor(const LineColor& color, int priority) {
    if (priority >= m_lineColorPriority) {
        m_lineColor = color;
        m_lineColorPriority = priority;
    }
}

int PreviewLockLine::getLineColorPriority() const {
    return m_lineColorPriority;
}

const LineColor& PreviewLockLine::getLineColor() const {
    return m_lineColor;
}

void PreviewLockLine::setLineWidth(float width, int priority) {
    if (priority >= m_lineWidthPriority) {
        m_lineWidth = width;
        m_lineWidthPriority = priority;
    }
}

int PreviewLockLine::getLineWidthPriority() const {
    return m_lineWidthPriority;
}

float PreviewLockLine::getLineWidth() const {
    return m_lineWidth;
}