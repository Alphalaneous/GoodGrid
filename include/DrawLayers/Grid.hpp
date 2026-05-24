#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../GradientColor.hpp"

namespace good_grid {
    
    class GOOD_GRID_API_DLL Grid : public DrawGridBase {
    public:
        static Grid* create();
        void draw(float minX, float maxX, float minY, float maxY) override;

        void setGridColor(const GradientColor& color);
        const GradientColor& getGridColor() const;

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