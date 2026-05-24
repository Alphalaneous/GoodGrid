#pragma once

#include "Export.hpp"
#include <ccTypes.h>
#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/binding/DrawGridLayer.hpp>
#include "GradientColor.hpp"

namespace good_grid {

    class GOOD_GRID_API_DLL DrawGridBase : public cocos2d::CCNode {
    public:
        enum class BlendMode {
            ADDITIVE,
            MULTIPLY,
            INVERT
        };

        static DrawGridBase* create();

        virtual void draw(float minX, float maxX, float minY, float maxY);

        void drawLine(const cocos2d::ccVertex2F& start, const cocos2d::ccVertex2F& end, const GradientColor& color, float width, BlendMode mode = BlendMode::ADDITIVE);
        void drawRect(const cocos2d::CCRect& rect, const GradientColor& color, BlendMode mode = BlendMode::ADDITIVE);
        void drawRectOutline(const cocos2d::CCRect& rect, const GradientColor& color, float width, BlendMode mode = BlendMode::ADDITIVE);
    
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