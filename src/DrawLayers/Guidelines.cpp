#include "../../include/DrawLayers/Guidelines.hpp"
#include "../Utils.hpp"

using namespace good_grid;

class Guidelines::Impl final {
public:
    utils::PriorityCallbackList<GuidelineCallback> m_colorsForValue;
};

Guidelines::Guidelines() : m_impl(std::make_unique<Impl>()) {}

Guidelines::~Guidelines() {}

Guidelines* Guidelines::create() {
    auto ret = new Guidelines();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Guidelines::init() {
    if (!CCNode::init()) return false;
    setID("guidelines"_spr);
    return true;
}

void Guidelines::draw(float minX, float maxX, float minY, float maxY) {
    if (!GameManager::get()->m_showSongMarkers) return;

    m_impl->m_colorsForValue.rebuildIfNeeded();

    for (const auto& [k, v] : getTimeMarkers()) {
        cocos2d::ccColor4B color = v;
        float x = k;
        float lineWidth = 1.0f;

        for (auto& fn : m_impl->m_colorsForValue.flat) {
            fn(color, x, lineWidth);
        }

        if (x < minX || x > maxX) continue;
        drawLine({x, minY}, {x, maxY}, color, lineWidth);
    }
}

void Guidelines::setPropertiesForObject(GuidelineCallback colorForValue, int priority) {
    m_impl->m_colorsForValue.add(std::move(colorForValue), priority);
}