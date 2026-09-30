#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../Color.hpp"

namespace alpha::grid {
    
class GOOD_GRID_API_DLL GuideObjects : public DrawGridBase {
public:
    using GuideObjectCallback = std::function<void(Color& bottomColor, Color& topColor, EffectGameObject* object, float& lineWidthBottom, float& lineWidthTop)>;

    static GuideObjects* create();
    void draw(const cocos2d::CCRect& visibleRect) override;

    void setPropertiesForObject(GuideObjectCallback colorForObject, int priority = 0);
    cocos2d::CCPoint getPortalMinMax(GameObject* obj);

protected:
    bool init() override;

    GuideObjects();
    ~GuideObjects();

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}