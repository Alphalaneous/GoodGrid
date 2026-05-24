#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"

namespace good_grid {
    
    class GOOD_GRID_API_DLL Ground : public DrawGridBase {
    public:
        static Ground* create();
        void draw(float minX, float maxX, float minY, float maxY) override;

        void setTopGroundColor(const cocos2d::ccColor4B& color);
        void setBottomGroundColor(const cocos2d::ccColor4B& color);

        const cocos2d::ccColor4B& getTopGroundColor() const;
        const cocos2d::ccColor4B& getBottomGroundColor() const;

        void setTopGroundLineWidth(float width);
        void setBottomGroundLineWidth(float width);

        float getTopGroundLineWidth() const;
        float getBottomGroundLineWidth() const;

        float getMinPortalY();
        float getMaxPortalY();

    protected:
        bool init() override;

        Ground();
        ~Ground();

        class Impl;
        std::unique_ptr<Impl> m_impl;
    };

}