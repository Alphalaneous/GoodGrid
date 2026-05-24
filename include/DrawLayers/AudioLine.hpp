#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"

namespace good_grid {
    
    class GOOD_GRID_API_DLL AudioLine : public DrawGridBase {
    public:
        using AudioLineCallback = std::function<void(cocos2d::ccColor4B& color, bool playback, float time, const cocos2d::CCPoint& position, float& lineWidth)>;

        static AudioLine* create();
        void draw(float minX, float maxX, float minY, float maxY) override;

        void setPropertiesForObject(AudioLineCallback colorForTime, int priority = 0);

    protected:
        bool init() override;

        AudioLine();
        ~AudioLine();

        class Impl;
        std::unique_ptr<Impl> m_impl;
    };

}