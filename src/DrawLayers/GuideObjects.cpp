#include "../../include/DrawLayers/GuideObjects.hpp"
#include "../Utils.hpp"
#include <Geode/utils/cocos.hpp>

using namespace good_grid;

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

void GuideObjects::draw(float minX, float maxX, float minY, float maxY) {
    if (getDrawGridLayer()->m_editorLayer->m_playbackMode == PlaybackMode::Playing) return;

    m_impl->m_colorsForObject.rebuildIfNeeded();

    for (auto obj : geode::cocos::CCArrayExt<EffectGameObject*>(getDrawGridLayer()->m_guideObjects)) {
        if (!isObjectVisible(obj)) continue;
        auto [y1, y2] = getPortalMinMax(obj);

        static const auto defaultLineColor = cocos2d::ccColor4B{0, 255, 255, 255};

        cocos2d::ccColor4B bottomColor = defaultLineColor;
        cocos2d::ccColor4B topColor = defaultLineColor;

        float lineWidthBottom = 2.0f;
        float lineWidthTop = 2.0f;

        for (auto& fn : m_impl->m_colorsForObject.flat) {
            fn(bottomColor, topColor, obj, lineWidthBottom, lineWidthTop);
        }

        if (y1 >= minY && y1 <= maxY) drawLine({minX, y1}, {maxX, y1}, bottomColor, lineWidthBottom);
        if (y2 >= minY && y2 <= maxY) drawLine({minX, y2}, {maxX, y2}, topColor, lineWidthTop);
    }
}

void GuideObjects::setPropertiesForObject(GuideObjectCallback colorForObject, int priority) {
    m_impl->m_colorsForObject.add(std::move(colorForObject), priority);
}

cocos2d::CCPoint GuideObjects::getPortalMinMax(GameObject* obj) {
    GameObjectType objectType = obj->m_objectType;
    static constexpr float defaultHeight = 300.0f;
    static constexpr float ballPortalHeight = 240.0f;
    static constexpr float spiderPortalHeight = 270.0f;
    static constexpr float gridStep = 30.0f;
    static constexpr float minYClamp = 90.0f;
    
    float height = defaultHeight;
    switch (obj->m_objectType) {
        case GameObjectType::BallPortal: height = ballPortalHeight; break;
        case GameObjectType::SpiderPortal: height = spiderPortalHeight; break;
        default: break;
    }
    
    float posY = obj->getPositionY();
    float yMin = std::floor((posY - height * 0.5f) / gridStep) * gridStep;
    if (yMin < minYClamp) yMin = minYClamp;
    return cocos2d::CCPoint{ yMin, yMin + height };
}