#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../GradientColor.hpp"

namespace good_grid {
    
    class GOOD_GRID_API_DLL DurationLines : public DrawGridBase {
    public:
        using DurationLineCallback = std::function<void(GradientColor& color, EffectGameObject* object, float& lineWidth)>;

        static DurationLines* create();
        void draw(float minX, float maxX, float minY, float maxY) override;

        void setPropertiesForObject(DurationLineCallback colorForObject, int priority = 0);

    protected:
        bool init() override;

        DurationLines();
        ~DurationLines();

        class Impl;
        std::unique_ptr<Impl> m_impl;
    };

}