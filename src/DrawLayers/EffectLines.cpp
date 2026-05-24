#include "../../include/DrawLayers/EffectLines.hpp"
#include "../Utils.hpp"
#include <Geode/utils/cocos.hpp>

using namespace good_grid;

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

void EffectLines::draw(float minX, float maxX, float minY, float maxY) {
    auto editorLayer = getDrawGridLayer()->m_editorLayer;

    if (!editorLayer->m_drawEffectLines || editorLayer->m_playbackMode == PlaybackMode::Playing) return;

    m_impl->m_colorsForObject.rebuildIfNeeded();

    for (auto obj : geode::cocos::CCArrayExt<EffectGameObject*>(getDrawGridLayer()->m_effectGameObjects)) {
        if (obj->m_isSpawnTriggered || obj->m_isTouchTriggered || !isObjectVisible(obj)) continue;
        float x = obj->getPositionX();
        if (x < minX || x > maxX || x < 0) continue;

        static const auto defaultLineColor = cocos2d::ccColor4B{0, 255, 255, 255};

        cocos2d::ccColor4B color = defaultLineColor;

        float lineWidth = 1.0f;

        for (auto& fn : m_impl->m_colorsForObject.flat) {
            fn(color, x, obj, lineWidth);
        }

        drawLine({x, minY}, { x, maxY}, color, lineWidth);
    }
}

void EffectLines::setPropertiesForObject(EffectLineCallback colorForObject, int priority) {
    m_impl->m_colorsForObject.add(std::move(colorForObject), priority);
}