#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../Color.hpp"

namespace alpha::grid {

class GOOD_GRID_API_DLL AudioLine : public DrawGridBase {
public:
    using AudioLineCallback = std::function<void(Color& color, bool playback, float time, const cocos2d::CCPoint& position, float& lineWidth)>;

    static AudioLine* create();
    void draw(float minX, float maxX, float minY, float maxY) override;

    void setPropertiesForTime(AudioLineCallback colorForTime, int priority = 0);

protected:
    bool init() override;

    AudioLine();
    ~AudioLine();

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}