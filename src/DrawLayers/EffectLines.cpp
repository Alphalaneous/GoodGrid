#include "../../include/DrawLayers/EffectLines.hpp"
#include "../Utils.hpp"
#include <Geode/utils/cocos.hpp>

namespace alpha::grid {

class EffectLines::Impl final {
public:
    utils::PriorityCallbackList<EffectLineCallback> m_colorsForObject;
};

EffectLines::EffectLines() : m_impl(std::make_unique<Impl>()) {}

EffectLines::~EffectLines() {}

EffectLines* EffectLines::create() {
    auto ret = new EffectLines();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool EffectLines::init() {
    if (!CCNode::init()) return false;
    setID("effect-lines"_spr);
    return true;
}

void EffectLines::draw(const cocos2d::CCRect& visibleRect) {
    auto editorLayer = getDrawGridLayer()->m_editorLayer;

    if (!editorLayer->m_drawEffectLines || editorLayer->m_playbackMode == PlaybackMode::Playing) return;

    m_impl->m_colorsForObject.rebuildIfNeeded();

    for (auto obj : geode::cocos::CCArrayExt<EffectGameObject*>(getDrawGridLayer()->m_effectGameObjects)) {
        if (obj->m_isSpawnTriggered || obj->m_isTouchTriggered || !isObjectVisible(obj)) continue;
        
        float x = obj->getPositionX();
        if (x < visibleRect.getMinX() || x > visibleRect.getMaxX() || x < 0.f) continue;

        static const auto defaultLineColor = Color{0, 255, 255, 255};

        Color color = defaultLineColor;

        float lineWidth = 1.f;

        for (auto& fn : m_impl->m_colorsForObject.flat) {
            fn(color, x, obj, lineWidth);
        }

        drawLine({x, visibleRect.getMinY()}, {x, visibleRect.getMaxY()}, color, lineWidth);
    }
}

void EffectLines::setPropertiesForObject(EffectLineCallback colorForObject, int priority) {
    m_impl->m_colorsForObject.add(std::move(colorForObject), priority);
}

}