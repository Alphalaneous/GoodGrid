#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../GradientColor.hpp"

namespace good_grid {
    
    class GOOD_GRID_API_DLL Bounds : public DrawGridBase {
    public:
        static Bounds* create();
        void draw(float minX, float maxX, float minY, float maxY) override;

        void setTopBoundColor(const GradientColor& color);
        void setBottomBoundColor(const GradientColor& color);
        void setVerticalBoundColor(const GradientColor& color);

        const GradientColor& getTopBoundColor() const;
        const GradientColor& getBottomBoundColor() const;
        const GradientColor& getVerticalColor() const;

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