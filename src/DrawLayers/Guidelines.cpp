#include "../../include/DrawLayers/Guidelines.hpp"
#include "../Utils.hpp"

namespace alpha::grid {

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

void Guidelines::draw(const cocos2d::CCRect& visibleRect) {
    if (!GameManager::get()->m_showSongMarkers) return;

    m_impl->m_colorsForValue.rebuildIfNeeded();

    for (const auto& [k, v] : getTimeMarkers()) {
        Color color = v;
        float x = k;
        float lineWidth = 1.f;

        for (auto& fn : m_impl->m_colorsForValue.flat) {
            fn(color, x, lineWidth);
        }

        if (x < visibleRect.getMinX() || x > visibleRect.getMaxX()) continue;
        
        drawLine({x, visibleRect.getMinY()}, {x, visibleRect.getMaxY()}, color, lineWidth);
    }
}

void Guidelines::setPropertiesForValue(GuidelineCallback colorForValue, int priority) {
    m_impl->m_colorsForValue.add(std::move(colorForValue), priority);
}

}