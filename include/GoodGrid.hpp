#pragma once

#include "Geode/cocos/cocoa/CCGeometry.h"
#include <Geode/loader/Event.hpp>
#include <Geode/loader/Dispatch.hpp>

#define MY_MOD_ID "alphalaneous.good_grid"

namespace good_grid {
    inline void markDirty()
    GEODE_EVENT_EXPORT_NORES(&markDirty, ());

    inline void setVanillaDraw(bool enabled)
    GEODE_EVENT_EXPORT_NORES(&setVanillaDraw, (enabled));

    inline void overrideGridBoundsSize(cocos2d::CCSize size)
    GEODE_EVENT_EXPORT_NORES(&overrideGridBoundsSize, (size));

    inline void overrideGridBoundsOrigin(cocos2d::CCPoint point)
    GEODE_EVENT_EXPORT_NORES(&overrideGridBoundsOrigin, (point));

    inline cocos2d::CCSize getGridBoundsSize()
    GEODE_EVENT_EXPORT_NORES(&getGridBoundsSize, ());

    inline cocos2d::CCPoint getGridBoundsOrigin()
    GEODE_EVENT_EXPORT_NORES(&getGridBoundsOrigin, ());

    inline cocos2d::CCSize getWorldViewSize()
    GEODE_EVENT_EXPORT_NORES(&getWorldViewSize, ());

    inline float getOverdrawFactor()
    GEODE_EVENT_EXPORT_NORES(&getOverdrawFactor, ());

    inline bool isDirty()
    GEODE_EVENT_EXPORT_NORES(&isDirty, ());

    inline bool isVanillaDraw()
    GEODE_EVENT_EXPORT_NORES(&isVanillaDraw, ());

    inline bool isObjectVisible(GameObject* object)
    GEODE_EVENT_EXPORT_NORES(&isObjectVisible, (object));

    inline void generateTimeMarkers()
    GEODE_EVENT_EXPORT_NORES(&generateTimeMarkers, ());

    inline std::unordered_map<float, cocos2d::ccColor4B> getTimeMarkers()
    GEODE_EVENT_EXPORT_NORES(&getTimeMarkers, ());
}
