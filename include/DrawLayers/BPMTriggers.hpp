#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../Color.hpp"

namespace alpha::grid {
    
class GOOD_GRID_API_DLL BPMTriggers : public DrawGridBase {
public:
    using BPMTriggerCallback = geode::Function<void(Color& color, AudioLineGuideGameObject* object, float& x, int beat, int beatsPerBar, float& lineWidth)>;

    static BPMTriggers* create();
    void draw(const cocos2d::CCRect& visibleRect) override;

    void setPropertiesForBeats(geode::ZStringView ID, BPMTriggerCallback colorsForBeats, int priority = 0);
    void removePropertiesForBeats(geode::ZStringView ID);

    static const Color& getDefaultBeatColor();
    static const Color& getDefaultPerBarBeatColor();

    static float getDefaultLineWidth();

protected:
    bool init() override;

    BPMTriggers();
    ~BPMTriggers();

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}