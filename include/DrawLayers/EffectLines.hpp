#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../GradientColor.hpp"

namespace good_grid {
    
    class GOOD_GRID_API_DLL EffectLines : public DrawGridBase {
    public:
        using EffectLineCallback = std::function<void(GradientColor& color, float& x, EffectGameObject* object, float& lineWidth)>;

        static EffectLines* create();
        void draw(float minX, float maxX, float minY, float maxY) override;

        void setPropertiesForObject(EffectLineCallback colorForObject, int priority = 0);

    protected:
        bool init() override;

        EffectLines();
        ~EffectLines();

        class Impl;
        std::unique_ptr<Impl> m_impl;
    };

}