#include "../../include/DrawLayers/DurationLines.hpp"
#include "../Utils.hpp"
#include <Geode/utils/cocos.hpp>

using namespace good_grid;

class DurationLines::Impl final {
public:
    utils::PriorityCallbackList<DurationLineCallback> m_colorsForObject;
    geode::Ref<GameObject> m_lastSnappedObject = nullptr;
};

DurationLines::DurationLines() : m_impl(std::make_unique<Impl>()) {}

DurationLines::~DurationLines() {}

DurationLines* DurationLines::create() {
    auto ret = new DurationLines();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool DurationLines::init() {
    if (!CCNode::init()) return false;
    setID("duration-lines"_spr);
    return true;
}

void DurationLines::draw(float minX, float maxX, float minY, float maxY) {
    auto editorLayer = getDrawGridLayer()->m_editorLayer;

    if (!editorLayer->m_showDurationLines || editorLayer->m_playbackMode == PlaybackMode::Playing) return;

    m_impl->m_colorsForObject.rebuildIfNeeded();

    const auto& startSpeed = editorLayer->m_levelSettings->m_startSpeed;
    const auto& isPlatformer = editorLayer->m_isPlatformer;
    auto& rotateChannel = editorLayer->m_gameState.m_rotateChannel;
    const auto& updateTimeMarkers = getDrawGridLayer()->m_updateTimeMarkers;
    auto speedObjects = getDrawGridLayer()->m_speedObjects;
    auto snapObject = editorLayer->m_editorUI->m_snapObject;

    for (auto obj : geode::cocos::CCArrayExt<EffectGameObject*>(editorLayer->m_durationObjects)) {
        if (!isObjectVisible(obj)) continue;
        
        static const auto defaultLineColor = GradientColor{100, 100, 100, 75};

        GradientColor color = defaultLineColor;

        float lineWidth = 2.0f;

        for (auto& fn : m_impl->m_colorsForObject.flat) {
            fn(color, obj, lineWidth);
        }

        cocos2d::CCPoint& endPos = obj->m_endPosition;

        if (updateTimeMarkers || (obj == m_impl->m_lastSnappedObject && !snapObject)) {
            endPos = cocos2d::CCPoint{0, 0};
            m_impl->m_lastSnappedObject = nullptr;
        }
        else {
            m_impl->m_lastSnappedObject = snapObject;
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
        
        const cocos2d::CCPoint& currentPos = obj->getPosition();

        if (!obj->m_isSpawnTriggered) {
            if (endPos == cocos2d::CCPointZero) {
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

                cocos2d::CCPoint newPos = LevelTools::posForTimeInternal(
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

        drawLine({currentPos.x, currentPos.y}, {endPos.x, endPos.y}, color, lineWidth);
    }
    getDrawGridLayer()->m_updateTimeMarkers = false;
}

void DurationLines::setPropertiesForObject(DurationLineCallback colorForObject, int priority) {
    m_impl->m_colorsForObject.add(std::move(colorForObject), priority);
}