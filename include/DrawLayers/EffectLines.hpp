#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../Color.hpp"

namespace alpha::grid {
    
class GOOD_GRID_API_DLL EffectLines : public DrawGridBase {
public:
    using EffectLineCallback = geode::Function<void(Color& color, float& x, EffectGameObject* object, float& lineWidth)>;

    static EffectLines* create();
    void draw(const cocos2d::CCRect& visibleRect) override;

    void setPropertiesForObject(geode::ZStringView ID, EffectLineCallback colorForObject, int priority = 0);
    void removePropertiesForObject(geode::ZStringView ID);

    static const Color& getDefaultLineColor();
    static float getDefaultLineWidth();

protected:
    bool init() override;

    EffectLines();
    ~EffectLines();

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}