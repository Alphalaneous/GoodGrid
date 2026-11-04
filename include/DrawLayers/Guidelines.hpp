#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../Color.hpp"

namespace alpha::grid {
    
class GOOD_GRID_API_DLL Guidelines : public DrawGridBase {
public:
    using GuidelineCallback = geode::Function<void(Color& color, float& value, float& lineWidth)>;

    static Guidelines* create();
    void draw(const cocos2d::CCRect& visibleRect) override;

    void setPropertiesForValue(geode::ZStringView ID, GuidelineCallback colorForValue, int priority = 0);
    void removePropertiesForValue(geode::ZStringView ID);

    static const Color& getColorA();
    static const Color& getColorB();
    static const Color& getColorC();
    static const Color& getColorD();

protected:
    bool init() override;

    Guidelines();
    ~Guidelines();

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}