#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../GradientColor.hpp"

namespace good_grid {
    
    class GOOD_GRID_API_DLL Guidelines : public DrawGridBase {
    public:
        using GuidelineCallback = std::function<void(GradientColor& color, float& value, float& lineWidth)>;

        static Guidelines* create();
        void draw(float minX, float maxX, float minY, float maxY) override;

        void setPropertiesForObject(GuidelineCallback colorForValue, int priority = 0);

    protected:
        bool init() override;

        Guidelines();
        ~Guidelines();

        class Impl;
        std::unique_ptr<Impl> m_impl;
    };

}