#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"

namespace good_grid {
    
    class GOOD_GRID_API_DLL PositionLines : public DrawGridBase {
    public:
        static PositionLines* create();
        void draw(float minX, float maxX, float minY, float maxY) override;

        void setVerticalLineColor(const cocos2d::ccColor4B& color);
        void setHorizontalLineColor(const cocos2d::ccColor4B& color);

        const cocos2d::ccColor4B& getVerticalLineColor() const;
        const cocos2d::ccColor4B& getHorizontalLineColor() const;

        void setVerticalLineWidth(float width);
        void setHorizontalLineWidth(float width);

        float getVerticalLineWidth() const;
        float getHorizontalLineWidth() const;

    protected:
        bool init() override;
        bool posLinesEnabledBE();

        PositionLines();
        ~PositionLines();

        class Impl;
        std::unique_ptr<Impl> m_impl;
    };

}