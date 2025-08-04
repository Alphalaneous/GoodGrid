#include "../include/DrawLayers.hpp"
#include "../include/DrawGridAPI.hpp"

using namespace geode::prelude;

void Grid::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    if (!dgl->m_editorLayer->m_showGrid || (dgl->m_editorLayer->m_hideGridOnPlay && dgl->m_editorLayer->m_playbackMode == PlaybackMode::Playing)) return;
    const CCSize size = DrawGridAPI::get().getGridBoundsSize();
    const CCPoint origin = DrawGridAPI::get().getGridBoundsOrigin();

    const float scale = dgl->m_editorLayer->m_objectLayer->getScale();
    const float xStart = std::max(minX - dgl->m_gridSize, origin.x);
    const float xEnd   = std::min(maxX + dgl->m_gridSize, size.width);
    
    const int firstGridX = static_cast<int>(std::floor(xStart / dgl->m_gridSize));
    const int lastGridX  = static_cast<int>(std::ceil(xEnd / dgl->m_gridSize));
    
    const float yStart = std::max(minY - dgl->m_gridSize, origin.y);
    const float yEnd   = std::min(maxY + dgl->m_gridSize, (dgl->m_editorLayer->m_levelSettings->m_dynamicLevelHeight ? size.height : MAX_HEIGHT) + GROUND_OFFSET);
    
    const int firstGridY = static_cast<int>(std::floor(yStart / dgl->m_gridSize));
    const int lastGridY  = static_cast<int>(std::ceil(yEnd / dgl->m_gridSize));
    
    for (int i = firstGridX; i <= lastGridX; ++i) {
        float x = i * dgl->m_gridSize;
        if (x < minX || x > maxX) continue;
        DrawGridAPI::get().drawLine({x, minY}, {x, maxY}, m_gridColor, 1.0f);
    }
    for (int i = firstGridY; i <= lastGridY; ++i) {
        float y = i * dgl->m_gridSize;
        if (y < minY || y > maxY) continue;
        DrawGridAPI::get().drawLine({minX, y}, {maxX, y}, m_gridColor, 1.0f);
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

void Bounds::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    DrawGridAPI::get().drawLine({0, minY}, {0, maxY}, m_verticalBoundColor, 1.0f);
    if (!dgl->m_editorLayer->m_showGround) {
        DrawGridAPI::get().drawLine({minX, GROUND_OFFSET}, {maxX, GROUND_OFFSET}, m_bottomBoundColor, 2.0f);
    }
    DrawGridAPI::get().drawLine({minX, maxY}, {maxX, maxY}, m_topBoundColor, 2.0f);
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

void Ground::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    if (dgl->m_editorLayer->m_playbackMode != PlaybackMode::Playing) return;

    if (dgl->m_editorLayer->m_player1->isFlying()) {
        float minPortalY = dgl->m_editorLayer->getMinPortalY();
        float maxPortalY = dgl->m_editorLayer->getMaxPortalY();

        DrawGridAPI::get().drawLine({minX, minPortalY}, {maxX, minPortalY}, m_bottomGroundColor, 2.0f);
        DrawGridAPI::get().drawLine({minX, maxPortalY}, {maxX, maxPortalY}, m_topGroundColor, 2.0f);
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

void GuideObjects::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    if (dgl->m_editorLayer->m_playbackMode == PlaybackMode::Playing) return;

    for (auto obj : CCArrayExt<EffectGameObject*>(dgl->m_guideObjects)) {
        if (!DrawGridAPI::get().isObjectVisible(obj)) continue;
        auto [y1, y2] = DrawGridAPI::get().getPortalMinMax(obj);

        LineColor bottomColor = { 0, 255,255,255 };
        LineColor topColor = { 0, 255,255,255 };

        for (const auto& [_, vec] : m_colorsForObject) {
            for (const auto& fn : vec) {
                fn(bottomColor, topColor, obj);
            }
        }

        if (y1 >= minY && y1 <= maxY) DrawGridAPI::get().drawLine({minX, y1}, {maxX, y1}, bottomColor, 2.0f);
        if (y2 >= minY && y2 <= maxY) DrawGridAPI::get().drawLine({minX, y2}, {maxX, y2}, topColor, 2.0f);
    }
}

void GuideObjects::setColorForObject(std::function<void(LineColor& bottomColor, LineColor& topColor, EffectGameObject* object)> colorForObject, int priority) {
    m_colorsForObject[priority].push_back(colorForObject);
}

void EffectLines::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    if (!dgl->m_editorLayer->m_drawEffectLines || dgl->m_editorLayer->m_playbackMode == PlaybackMode::Playing) return;
    for (auto obj : CCArrayExt<EffectGameObject*>(dgl->m_effectGameObjects)) {
        if (obj->m_isSpawnTriggered || obj->m_isTouchTriggered || !DrawGridAPI::get().isObjectVisible(obj)) continue;
        float x = obj->getPositionX();
        if (x < minX || x > maxX || x < 0) continue;

        LineColor color = { 0, 255,255,255 };

        for (const auto& [_, vec] : m_colorsForObject) {
            for (const auto& fn : vec) {
                fn(color, obj);
            }
        }

        DrawGridAPI::get().drawLine({x, minY}, { x, maxY}, color, 1.0f);
    }
}

void EffectLines::setColorForObject(std::function<void(LineColor& color, EffectGameObject* object)> colorForObject, int priority) {
    m_colorsForObject[priority].push_back(colorForObject);
}

void DurationLines::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    if (!dgl->m_editorLayer->m_showDurationLines || dgl->m_editorLayer->m_playbackMode == PlaybackMode::Playing) return;
    for (auto obj : CCArrayExt<EffectGameObject*>(dgl->m_editorLayer->m_durationObjects)) {
        if (!DrawGridAPI::get().isObjectVisible(obj)) continue;
        
        LineColor color = { 255, 255, 255, 115};

        for (const auto& [_, vec] : m_colorsForObject) {
            for (const auto& fn : vec) {
                fn(color, obj);
            }
        }

        CCPoint& endPos = obj->m_endPosition;

        if (dgl->m_updateTimeMarkers) {
            endPos = CCPoint{0, 0};
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
                    dgl->m_speedObjects,
                    (int)dgl->m_editorLayer->m_levelSettings->m_startSpeed,
                    obj->m_ordValue,
                    obj->m_channelValue,
                    false,
                    dgl->m_editorLayer->m_isPlatformer,
                    true,
                    false,
                    false
                );

                bool wasRotated = LevelTools::getLastGameplayRotated();

                CCPoint newPos = LevelTools::posForTimeInternal(
                    currentTime + time,
                    dgl->m_speedObjects,
                    (int)dgl->m_editorLayer->m_levelSettings->m_startSpeed,
                    dgl->m_editorLayer->m_isPlatformer,
                    false,
                    true,
                    dgl->m_editorLayer->m_gameState.m_rotateChannel,
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

        DrawGridAPI::get().drawLine({currentPos.x, currentPos.y}, {endPos.x, endPos.y}, color, 2.0f, false);
    }
}

void DurationLines::setColorForObject(std::function<void(LineColor& color, EffectGameObject* object)> colorForObject, int priority) {
    m_colorsForObject[priority].push_back(colorForObject);
}

void Guidelines::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    if (!GameManager::get()->m_showSongMarkers) return;
    for (const auto& [k, v] : DrawGridAPI::get().getTimeMarkers()) {
        if (k < minX || k > maxX) continue;
        LineColor color = v;

        for (const auto& [_, vec] : m_colorsForValue) {
            for (const auto& fn : vec) {
                fn(color, k);
            }
        }

        DrawGridAPI::get().drawLine({k, minY}, {k, maxY}, color, 1.0f);
    }
}

void Guidelines::setColorForValue(std::function<void(LineColor& color, float value)> colorForValue, int priority) {
    m_colorsForValue[priority].push_back(colorForValue);
}

void BPMTriggers::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    for (auto& [_, obj] : dgl->m_audioLineObjects) {
        if (obj->m_disabled || !DrawGridAPI::get().isObjectVisible(obj)) continue;
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
        
        if (endX < minX || startX > maxX) continue;
        const int beatsPerBar = obj->m_beatsPerBar;
        
        if (obj->m_beatsPerMinute == 0 || beatsPerBar == 0) continue;
        const float timeStep = speed * 60.f / (obj->m_beatsPerMinute * beatsPerBar);
       
        int beatStart = std::max(0, static_cast<int>(std::floor((minX - startX) / timeStep)));
        int beatEnd = static_cast<int>(std::ceil((maxX - startX) / timeStep));
        
        for (int beat = beatStart; beat <= beatEnd; ++beat) {
            float x = startX + timeStep * beat;
            if (x < minX || x > maxX) continue;
            if (x > endX || beat > beatEnd) break;

            LineColor color;
            if (beat % beatsPerBar == 0) {
                color = {255,255,0,255};
            }
            else {
                color = {255,127,0,255};
            }

            for (const auto& [_, vec] : m_colorsForBeats) {
                for (const auto& fn : vec) {
                    fn(color, obj, beat, beatsPerBar);
                }
            }

            DrawGridAPI::get().drawLine({x, minY}, {x, maxY}, color, 1.0f);
        }
    }
}

void BPMTriggers::setColorForBeats(std::function<void(LineColor& color, AudioLineGuideGameObject* object, int beat, int beatsPerBar)> colorForBeats, int priority) {
    m_colorsForBeats[priority].push_back(colorForBeats);
}

void AudioLine::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    LineColor color = {50, 255, 50, 255};

    float width = 5.0f;
    if (dgl->m_editorLayer->m_playbackActive) {
        CCPoint pos = dgl->posForTime(dgl->m_playbackTime);
        dgl->m_playbackX = 0;
        dgl->m_playbackY = 0;

        if (LevelTools::getLastGameplayRotated()) {
            dgl->m_playbackY = pos.y;
        } else {
            dgl->m_playbackX = pos.x;
        }
    } else {
        color = {2, 255, 2, 100};
        width = 3.0f;
    }

    if (dgl->m_editorLayer->m_playbackMode == PlaybackMode::Playing) {
        dgl->m_playbackX = 0;
        dgl->m_playbackY = 0;
        dgl->m_playbackTime = 0;
    }

    for (const auto& [_, vec] : m_colorsForTime) {
        for (const auto& fn : vec) {
            fn(color, dgl->m_editorLayer->m_playbackActive, dgl->m_playbackTime, {dgl->m_playbackX, dgl->m_playbackY});
        }
    }

    if (dgl->m_playbackX != 0) {
        DrawGridAPI::get().drawLine({dgl->m_playbackX, minY}, {dgl->m_playbackX, maxY}, color, width, !dgl->m_editorLayer->m_playbackActive);
    }
    if (dgl->m_playbackY != 0) {
        DrawGridAPI::get().drawLine({minX, dgl->m_playbackY}, {maxX, dgl->m_playbackY}, color, width, !dgl->m_editorLayer->m_playbackActive);
    }
}

void AudioLine::setColorForTime(std::function<void(LineColor& color, bool playback, float time, const cocos2d::CCPoint& position)> colorForTime, int priority) {
    m_colorsForTime[priority].push_back(colorForTime);
}

void PositionLines::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    if (dgl->m_editorLayer->m_playbackMode == PlaybackMode::Playing)
        return;

    const CCSize& winSize = CCDirector::get()->getWinSize();
    float toolbarHeight = dgl->m_editorLayer->m_editorUI->m_toolbarHeight;

    auto objectLayer = dgl->m_editorLayer->m_objectLayer;

    CCPoint screenCenter = { winSize.width / 2, winSize.height / 2 };
    CCPoint pivotInObject = objectLayer->convertToNodeSpace(screenCenter);
    CCPoint lineScreenPos = { winSize.width / 2, (winSize.height + toolbarHeight) / 2 };
    CCPoint linePosInObject = objectLayer->convertToNodeSpace(lineScreenPos);

    float angle = CC_DEGREES_TO_RADIANS(dgl->m_editorLayer->m_gameState.m_cameraAngle);
    float s = std::sin(-angle);
    float c = std::cos(-angle);

    float dx = linePosInObject.x - pivotInObject.x;
    float dy = linePosInObject.y - pivotInObject.y;

    float rotatedX = c * dx - s * dy + pivotInObject.x;
    float rotatedY = s * dx + c * dy + pivotInObject.y;

    CCPoint finalPos = { rotatedX, rotatedY };

    if (finalPos.x >= minX && finalPos.x <= maxX) {
        DrawGridAPI::get().drawLine({finalPos.x, minY}, {finalPos.x, maxY}, m_verticalLineColor, 2);
    }

    if (finalPos.y >= minY && finalPos.y <= maxY) {
        DrawGridAPI::get().drawLine({minX, finalPos.y}, {maxX, finalPos.y}, m_horizontalLineColor, 2);
    }
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

void PreviewLockLine::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {
    if (dgl->m_editorLayer->m_playbackMode != PlaybackMode::Not) return;
    if (dgl->m_editorLayer->m_previewPosition.x <= 0) return;
    const CCPoint& pos = dgl->m_editorLayer->m_previewPosition;
    DrawGridAPI::get().drawLine({pos.x, minY}, {pos.x, maxY}, m_lineColor, 2);
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