#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"

namespace good_grid {
    
    class GOOD_GRID_API_DLL Grid : public DrawGridBase {
    public:
        static Grid* create();
        void draw(float minX, float maxX, float minY, float maxY) override;

        void setGridColor(const cocos2d::ccColor4B& color);
        const cocos2d::ccColor4B& getGridColor() const;

        void setLineWidth(float width);
        float getLineWidth() const;

        void setInverted(bool inverted);
        bool isInverted();

    protected:
        bool init() override;

        Grid();
        ~Grid();

        class Impl;
        std::unique_ptr<Impl> m_impl;
    };

}