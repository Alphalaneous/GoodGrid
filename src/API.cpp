#define GEODE_DEFINE_EVENT_EXPORTS

#include "../include/API.hpp"
#include <Geode/Geode.hpp>
#include "Utils.hpp"

using namespace geode::prelude;

namespace alpha::grid {

void setVanillaDraw(bool enabled) {
    auto dgl = utils::getDrawGridLayer();
    if (!dgl) return;

    dgl->getCustom()->setVanillaDraw(enabled);
}

void overrideGridBoundsSize(cocos2d::CCSize size) {
    auto dgl = utils::getDrawGridLayer();
    if (!dgl) return;

    dgl->getCustom()->overrideGridBoundsSize(size);
}

void overrideGridBoundsOrigin(cocos2d::CCPoint point) {
    auto dgl = utils::getDrawGridLayer();
    if (!dgl) return;

    dgl->getCustom()->overrideGridBoundsOrigin(point);
}

cocos2d::CCSize getGridBoundsSize() {
    auto dgl = utils::getDrawGridLayer();
    if (!dgl) return {};

    return dgl->getCustom()->getGridBoundsSize();
}

cocos2d::CCPoint getGridBoundsOrigin() {
    auto dgl = utils::getDrawGridLayer();
    if (!dgl) return {};

    return dgl->getCustom()->getGridBoundsOrigin();
}

bool isVanillaDraw() {
    auto dgl = utils::getDrawGridLayer();
    if (!dgl) return false;

    return dgl->getCustom()->isVanillaDraw();
}

bool isObjectVisible(GameObject* object) {
    auto dgl = utils::getDrawGridLayer();
    if (!dgl) return false;

    return dgl->getCustom()->isObjectVisible(object);
}

void generateTimeMarkers() {
    auto dgl = utils::getDrawGridLayer();
    if (!dgl) return;

    dgl->getCustom()->generateTimeMarkers();
}

std::unordered_map<float, cocos2d::ccColor4B> getTimeMarkers() {
    auto dgl = utils::getDrawGridLayer();
    if (!dgl) return {};

    return dgl->getCustom()->getTimeMarkers();
}

float getSin() {
    auto dgl = utils::getDrawGridLayer();
    if (!dgl) return 0.f;

    return dgl->getCustom()->getSin();
}

float getCos() {
    auto dgl = utils::getDrawGridLayer();
    if (!dgl) return 0.f;

    return dgl->getCustom()->getCos();
}

CCRect getVisibleBounds() {
    auto dgl = utils::getDrawGridLayer();
    if (!dgl) return {};

    return dgl->getCustom()->getVisibleBounds();
}

}