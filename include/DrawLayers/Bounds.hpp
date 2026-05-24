#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"

namespace good_grid {
    
    class GOOD_GRID_API_DLL Bounds : public DrawGridBase {
    public:
        static Bounds* create();
        void draw(float minX, float maxX, float minY, float maxY) override;

        void setTopBoundColor(const cocos2d::ccColor4B& color);
        void setBottomBoundColor(const cocos2d::ccColor4B& color);
        void setVerticalBoundColor(const cocos2d::ccColor4B& color);

        const cocos2d::ccColor4B& getTopBoundColor() const;
        const cocos2d::ccColor4B& getBottomBoundColor() const;
        const cocos2d::ccColor4B& getVerticalColor() const;

        void setTopBoundLineWidth(float width);
        void setBottomBoundLineWidth(float width);
        void setVerticalBoundLineWidth(float width);

        float getTopBoundLineWidth() const;
        float getBottomBoundLineWidth() const;
        float getVerticalLineWidth() const;

    protected:
        bool init() override;

        Bounds();
        ~Bounds();

        class Impl;
        std::unique_ptr<Impl> m_impl;
    };

}