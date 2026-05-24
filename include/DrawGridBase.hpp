#pragma once

#include "Export.hpp"
#include <ccTypes.h>
#include <Geode/cocos/base_nodes/CCNode.h>
#include <Geode/binding/DrawGridLayer.hpp>

namespace good_grid {

    class GOOD_GRID_API_DLL DrawGridBase : public cocos2d::CCNode {
    public:
        enum class DrawMode {
            ADDITIVE,
            MULTIPLY,
            INVERT
        };

        static DrawGridBase* create();

        virtual void draw(float minX, float maxX, float minY, float maxY);

        void drawLine(const cocos2d::ccVertex2F& start, const cocos2d::ccVertex2F& end, const cocos2d::ccColor4B& color, float width, DrawMode mode = DrawMode::ADDITIVE);
        void drawLine(const cocos2d::ccVertex2F& start, const cocos2d::ccVertex2F& end, const cocos2d::ccColor4B& colorA, const cocos2d::ccColor4B& colorB, float width, DrawMode mode = DrawMode::ADDITIVE);
        void drawRect(const cocos2d::CCRect& rect, const cocos2d::ccColor4B& color, DrawMode mode = DrawMode::ADDITIVE);
        void drawRectOutline(const cocos2d::CCRect& rect, const cocos2d::ccColor4B& color, float width, DrawMode mode = DrawMode::ADDITIVE);
    
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