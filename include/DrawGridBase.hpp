#pragma once

#include "Export.hpp"
#include <Geode/binding/DrawGridLayer.hpp>
#include "Color.hpp"

namespace alpha::grid {

class GOOD_GRID_API_DLL DrawGridBase : public cocos2d::CCNode {
public:
    static DrawGridBase* create();

    virtual void draw(const cocos2d::CCRect& visibleRect);

    void drawQuad(const cocos2d::CCPoint& v0, const cocos2d::CCPoint& v1, const cocos2d::CCPoint& v2, const cocos2d::CCPoint& v3, const Color& color, float angle = 0.f);
    void drawLine(const cocos2d::CCPoint& start, const cocos2d::CCPoint& end, const Color& color, float width, bool relative = false);
    void drawRect(const cocos2d::CCRect& rect, const Color& color);
    void drawRectOutline(const cocos2d::CCRect& rect, const Color& color, float width, bool relative = false);

    static constexpr float GroundOffset = 90.f;

    // These can change depending if another mod changes the bounds of the editor, it is recommended instead to use alpha::grid::getGridBoundsSize and alpha::grid::getGridBoundsOrigin for these

    static constexpr float MinWidth = -3000.f;
    static constexpr float MinHeight = -3000.f;

    static constexpr float MaxWidth = 240000.f;
    static constexpr float MaxHeight = 2490.f;
    static constexpr float MaxDynamicHeight = 30090.f;

private:
    void drawRectInternal(const cocos2d::CCRect& rect, const Color& color, float angle = 0.f);

protected:
    DrawGridBase();
    ~DrawGridBase();

    DrawGridLayer* getDrawGridLayer();
    bool isObjectVisible(GameObject* object);
    cocos2d::CCSize getGridBoundsSize();
    cocos2d::CCPoint getGridBoundsOrigin();
    const std::unordered_map<float, const Color&>& getTimeMarkers();

    void visit() override;
    void onEnter() override;

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}