#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../GradientColor.hpp"

namespace good_grid {
    
    class GOOD_GRID_API_DLL BPMTriggers : public DrawGridBase {
    public:
        using BPMTriggerCallback = std::function<void(GradientColor& color, AudioLineGuideGameObject* object, float& x, int beat, int beatsPerBar, float& lineWidth)>;

        static BPMTriggers* create();
        void draw(float minX, float maxX, float minY, float maxY) override;

        void setPropertiesForObject(BPMTriggerCallback colorsForBeats, int priority = 0);

    protected:
        bool init() override;

        BPMTriggers();
        ~BPMTriggers();

        class Impl;
        std::unique_ptr<Impl> m_impl;
    };

}