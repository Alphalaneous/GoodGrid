#pragma once

#include "Export.hpp"
#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/binding/DrawGridLayer.hpp>
#include "Color.hpp"

namespace alpha::grid {

class GOOD_GRID_API_DLL DrawGridBase : public cocos2d::CCNode {
public:
    static DrawGridBase* create();

    virtual void draw(const cocos2d::CCRect& visibleRect);

    void drawQuad(const cocos2d::ccVertex2F& v0, const cocos2d::ccVertex2F& v1, const cocos2d::ccVertex2F& v2, const cocos2d::ccVertex2F& v3, const Color& color, float angle = 0.f);
    void drawLine(const cocos2d::ccVertex2F& start, const cocos2d::ccVertex2F& end, const Color& color, float width, bool relative = false);
    void drawRect(const cocos2d::CCRect& rect, const Color& color);
    void drawRectOutline(const cocos2d::CCRect& rect, const Color& color, float width, bool relative = false);

private:
    void drawRectInternal(const cocos2d::CCRect& rect, const Color& color, float angle = 0.f);

protected:
    DrawGridBase();
    ~DrawGridBase();


    DrawGridLayer* getDrawGridLayer();
    bool isObjectVisible(GameObject* object);
    cocos2d::CCSize getGridBoundsSize();
    cocos2d::CCPoint getGridBoundsOrigin();
    const std::unordered_map<float, cocos2d::ccColor4B>& getTimeMarkers();

    void visit() override;
    void onEnter() override;

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}