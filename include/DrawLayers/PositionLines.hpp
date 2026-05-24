#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../GradientColor.hpp"

namespace good_grid {
    
    class GOOD_GRID_API_DLL PositionLines : public DrawGridBase {
    public:
        static PositionLines* create();
        void draw(float minX, float maxX, float minY, float maxY) override;

        void setVerticalLineColor(const GradientColor& color);
        void setHorizontalLineColor(const GradientColor& color);

        const GradientColor& getVerticalLineColor() const;
        const GradientColor& getHorizontalLineColor() const;

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