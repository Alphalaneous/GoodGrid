#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../Color.hpp"

namespace alpha::grid {
    
class GOOD_GRID_API_DLL DurationLines : public DrawGridBase {
public:
    using DurationLineCallback = geode::Function<void(Color& color, EffectGameObject* object, float& lineWidth)>;

    static DurationLines* create();
    void draw(const cocos2d::CCRect& visibleRect) override;

    void setPropertiesForObject(geode::ZStringView ID, DurationLineCallback colorForObject, int priority = 0);
    void removePropertiesForObject(geode::ZStringView ID);

    static const Color& getDefaultGridColor();
    static float getDefaultLineWidth();

protected:
    bool init() override;

    DurationLines();
    ~DurationLines();

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}