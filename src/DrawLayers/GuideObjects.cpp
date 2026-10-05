#include "../../include/DrawLayers/GuideObjects.hpp"
#include "../Utils.hpp"
#include <Geode/utils/cocos.hpp>

namespace alpha::grid {

class GuideObjects::Impl final {
public:
    utils::PriorityCallbackList<GuideObjectCallback> m_colorsForObject;
};

GuideObjects::GuideObjects() : m_impl(std::make_unique<Impl>()) {}

GuideObjects::~GuideObjects() {}

GuideObjects* GuideObjects::create() {
    auto ret = new GuideObjects();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool GuideObjects::init() {
    if (!CCNode::init()) return false;
    setID("guide-objects"_spr);
    return true;
}

void GuideObjects::draw(const cocos2d::CCRect& visibleRect) {
    if (getDrawGridLayer()->m_editorLayer->m_playbackMode == PlaybackMode::Playing) return;

    m_impl->m_colorsForObject.rebuildLazy();

    for (auto obj : geode::cocos::CCArrayExt<EffectGameObject*>(getDrawGridLayer()->m_guideObjects)) {
        if (!isObjectVisible(obj)) continue;

        auto [y1, y2] = getPortalMinMax(obj);

        Color bottomColor = getDefaultBottomColor();
        Color topColor = getDefaultTopColor();

        float lineWidthBottom = getDefaultBottomLineWidth();
        float lineWidthTop = getDefaultTopLineWidth();

        for (auto& fn : m_impl->m_colorsForObject.all()) {
            (*fn)(bottomColor, topColor, obj, lineWidthBottom, lineWidthTop);
        }

        if (y1 >= visibleRect.getMinY() && y1 <= visibleRect.getMaxY()) {
            drawLine({visibleRect.getMinX(), y1}, {visibleRect.getMaxX(), y1}, bottomColor, lineWidthBottom);
        }
        
        if (y2 >= visibleRect.getMinY() && y2 <= visibleRect.getMaxY()) {
            drawLine({visibleRect.getMinX(), y2}, {visibleRect.getMaxX(), y2}, topColor, lineWidthTop);
        }
    }
}

void GuideObjects::setPropertiesForObject(ZStringView ID, GuideObjectCallback colorForObject, int priority) {
    m_impl->m_colorsForObject.add(ID, std::move(colorForObject), priority);
}

void GuideObjects::removePropertiesForObject(geode::ZStringView ID) {
    m_impl->m_colorsForObject.remove(ID);
}

cocos2d::CCPoint GuideObjects::getPortalMinMax(GameObject* obj) {
    static constexpr float defaultHeight = 300.f;
    static constexpr float ballPortalHeight = 240.f;
    static constexpr float spiderPortalHeight = 270.f;
    static constexpr float gridStep = 30.f;
    static constexpr float minYClamp = 90.f;
    
    float height = defaultHeight;
    switch (obj->m_objectType) {
        case GameObjectType::BallPortal: {
            height = ballPortalHeight;
            break;
        }
        case GameObjectType::SpiderPortal: {
            height = spiderPortalHeight;
            break;
        }
        default: {
            break;
        }
    }
    
    float yMin = std::max(std::floor((obj->getPositionY() - height / 2.f) / gridStep) * gridStep, minYClamp);

    return {yMin, yMin + height};
}

const Color& GuideObjects::getDefaultTopColor() {
    static Color defaultColor = {0, 255, 255, 255};
    return defaultColor;
}

const Color& GuideObjects::getDefaultBottomColor() {
    static Color defaultColor = {0, 255, 255, 255};
    return defaultColor;
}

float GuideObjects::getDefaultTopLineWidth() {
    return 2.f;
}

float GuideObjects::getDefaultBottomLineWidth() {
    return 2.f;
}

}