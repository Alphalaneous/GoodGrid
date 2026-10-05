#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../Color.hpp"

namespace alpha::grid {
    
class GOOD_GRID_API_DLL GuideObjects : public DrawGridBase {
public:
    using GuideObjectCallback = geode::Function<void(Color& bottomColor, Color& topColor, EffectGameObject* object, float& lineWidthBottom, float& lineWidthTop)>;

    static GuideObjects* create();
    void draw(const cocos2d::CCRect& visibleRect) override;

    void setPropertiesForObject(geode::ZStringView ID, GuideObjectCallback colorForObject, int priority = 0);
    void removePropertiesForObject(geode::ZStringView ID);

    cocos2d::CCPoint getPortalMinMax(GameObject* obj);

    static const Color& getDefaultTopColor();
    static const Color& getDefaultBottomColor();

    static float getDefaultTopLineWidth();
    static float getDefaultBottomLineWidth();

protected:
    bool init() override;

    GuideObjects();
    ~GuideObjects();

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}