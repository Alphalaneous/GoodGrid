#pragma once

#include <Geode/cocos/cocoa/CCGeometry.h>
#include <Geode/loader/Event.hpp>
#include <Geode/loader/Dispatch.hpp>

#define MY_MOD_ID "alphalaneous.good_grid"

namespace alpha::grid {

GEODE_EVENT_EXPORT(void, overrideGridBoundsSize, (cocos2d::CCSize size), (size));

GEODE_EVENT_EXPORT(void, overrideGridBoundsOrigin, (cocos2d::CCPoint point), (point));

GEODE_EVENT_EXPORT(cocos2d::CCSize, getGridBoundsSize, (void), ());

GEODE_EVENT_EXPORT(cocos2d::CCPoint, getGridBoundsOrigin, (void), ());

GEODE_EVENT_EXPORT(bool, isObjectVisible, (GameObject* object), (object));

GEODE_EVENT_EXPORT(float, getSin, (void), ());

GEODE_EVENT_EXPORT(float, getCos, (void), ());

}
