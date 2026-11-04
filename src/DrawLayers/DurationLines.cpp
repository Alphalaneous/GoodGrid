#include "../../include/DrawLayers/DurationLines.hpp"
#include "../Utils.hpp"
#include <Geode/utils/cocos.hpp>

namespace alpha::grid {

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

void DurationLines::draw(const cocos2d::CCRect& visibleRect) {
    auto editorLayer = getDrawGridLayer()->m_editorLayer;

    if (!editorLayer->m_showDurationLines || editorLayer->m_playbackMode == PlaybackMode::Playing) return;

    m_impl->m_colorsForObject.rebuildLazy();

    auto startSpeed = editorLayer->m_levelSettings->m_startSpeed;
    auto isPlatformer = editorLayer->m_isPlatformer;
    auto& rotateChannel = editorLayer->m_gameState.m_rotateChannel;
    auto updateTimeMarkers = getDrawGridLayer()->m_updateTimeMarkers;
    auto speedObjects = getDrawGridLayer()->m_speedObjects;
    auto snapObject = editorLayer->m_editorUI->m_snapObject;

    for (auto obj : geode::cocos::CCArrayExt<EffectGameObject*>(editorLayer->m_durationObjects)) {
        if (!isObjectVisible(obj)) continue;
        
        Color color = getDefaultGridColor();

        float lineWidth = getDefaultLineWidth();;

        for (auto& fn : m_impl->m_colorsForObject.all()) {
            (*fn)(color, obj, lineWidth);
        }

        auto& endPos = obj->m_endPosition;

        if (updateTimeMarkers || (obj == m_impl->m_lastSnappedObject && !snapObject)) {
            endPos = cocos2d::CCPoint{0.f, 0.f};
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
            time = static_cast<SFXTriggerGameObject*>(obj)->m_soundDuration;
        }

        if (time <= 0.f) continue;
        
        auto currentPos = obj->getPosition();

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

                auto newPos = LevelTools::posForTimeInternal(
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

        if (endPos.x < visibleRect.getMinX() || currentPos.x > visibleRect.getMaxX() || endPos.y < visibleRect.getMinY() || currentPos.y > visibleRect.getMaxY()) continue;

        drawLine({currentPos.x, currentPos.y}, {endPos.x, endPos.y}, color, lineWidth);
    }
    getDrawGridLayer()->m_updateTimeMarkers = false;
}

void DurationLines::setPropertiesForObject(ZStringView ID, DurationLineCallback colorForObject, int priority) {
    m_impl->m_colorsForObject.add(ID, std::move(colorForObject), priority);
}

void DurationLines::removePropertiesForObject(geode::ZStringView ID) {
    m_impl->m_colorsForObject.remove(ID);
}

const Color& DurationLines::getDefaultGridColor() {
    static Color defaultColor = Color{100, 100, 100, 75};
    return defaultColor;
}

float DurationLines::getDefaultLineWidth() {
    return 2.f;
}

}