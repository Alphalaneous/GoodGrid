#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../Color.hpp"

namespace alpha::grid {

class GOOD_GRID_API_DLL AudioLine : public DrawGridBase {
public:
    using AudioLineCallback = geode::Function<void(Color& color, bool playback, float time, const cocos2d::CCPoint& position, float& lineWidth)>;

    static AudioLine* create();
    void draw(const cocos2d::CCRect& visibleRect) override;

    void setPropertiesForTime(geode::ZStringView ID, AudioLineCallback colorForTime, int priority = 0);
    void removePropertiesForTime(geode::ZStringView ID);

    static const Color& getDefaultActiveColor();
    static const Color& getDefaultInactiveColor();

    static float getDefaultActiveLineWidth();
    static float getDefaultInactiveLineWidth();

protected:
    bool init() override;

    AudioLine();
    ~AudioLine();

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}