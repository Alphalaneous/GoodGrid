#include "../../include/DrawLayers/BPMTriggers.hpp"
#include "../Utils.hpp"

namespace alpha::grid {

class BPMTriggers::Impl final {
public:
    utils::PriorityCallbackList<BPMTriggerCallback> m_colorsForBeats;
};

BPMTriggers::BPMTriggers() : m_impl(std::make_unique<Impl>()) {}

BPMTriggers::~BPMTriggers() {}

BPMTriggers* BPMTriggers::create() {
    auto ret = new BPMTriggers();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool BPMTriggers::init() {
    if (!CCNode::init()) return false;
    setID("bpm-triggers"_spr);
    return true;
}

void BPMTriggers::draw(const cocos2d::CCRect& visibleRect) {
    m_impl->m_colorsForBeats.rebuildLazy();

    for (auto& [_, obj] : getDrawGridLayer()->m_audioLineObjects) {
        if (obj->m_disabled || !isObjectVisible(obj)) continue;
        float speed = getDrawGridLayer()->m_normalSpeed;
        
        switch (obj->m_speed) {
            case Speed::Slow: {
                speed = getDrawGridLayer()->m_slowSpeed;
                break;
            }
            case Speed::Fast: {
                speed = getDrawGridLayer()->m_fastSpeed;
                break;
            }
            case Speed::Faster: {
                speed = getDrawGridLayer()->m_fasterSpeed;
                break;
            }
            case Speed::Fastest: {
                speed = getDrawGridLayer()->m_fastestSpeed;
                break;
            }
            default: {
                break;
            }
        }

        float startX = obj->getPositionX();
        float duration = obj->m_duration * speed;
        float endX = startX + duration;
        int beatsPerBar = obj->m_beatsPerBar;
        
        if (obj->m_beatsPerMinute == 0 || beatsPerBar == 0) continue;

        float timeStep = speed * 60.f / (obj->m_beatsPerMinute * beatsPerBar);
       
        int beatStart = std::max(0, static_cast<int>(std::floor((visibleRect.getMinX() - startX) / timeStep)));
        int beatEnd = static_cast<int>(std::ceil((visibleRect.getMaxX() - startX) / timeStep));
        
        for (int beat = beatStart; beat <= beatEnd; ++beat) {
            float x = startX + timeStep * beat;

            Color color;
            float lineWidth = getDefaultLineWidth();

            if (beat % beatsPerBar == 0) {
                color = getDefaultBeatColor();
            }
            else {
                color = getDefaultPerBarBeatColor();
            }

            for (auto& fn : m_impl->m_colorsForBeats.all()) {
                (*fn)(color, obj, x, beat, beatsPerBar, lineWidth);
            }

            if (x < visibleRect.getMinX() || x > visibleRect.getMaxX() || x > endX || beat > beatEnd) continue;
            
            drawLine({x, visibleRect.getMinY()}, {x, visibleRect.getMaxY()}, color, lineWidth);
        }
    }
}

void BPMTriggers::setPropertiesForBeats(ZStringView ID, BPMTriggerCallback colorsForBeats, int priority) {
    m_impl->m_colorsForBeats.add(ID, std::move(colorsForBeats), priority);
}

void BPMTriggers::removePropertiesForBeats(geode::ZStringView ID) {
    m_impl->m_colorsForBeats.remove(ID);
}

const Color& BPMTriggers::getDefaultBeatColor() {
    static Color defaultColor = {255, 255, 0, 255};
    return defaultColor;
}

const Color& BPMTriggers::getDefaultPerBarBeatColor() {
    static Color defaultColor = {255, 127, 0, 255};
    return defaultColor;
}

float BPMTriggers::getDefaultLineWidth() {
    return 1.f;
}

}