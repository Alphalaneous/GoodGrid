#define GEODE_DEFINE_EVENT_EXPORTS

#include "../include/GoodGrid.hpp"
#include <Geode/Geode.hpp>
#include "Utils.hpp"

using namespace geode::prelude;

namespace good_grid {

    void markDirty() {
        auto dgl = utils::getDrawGridLayer();
        if (!dgl) return;

        dgl->getCustom()->markDirty();
    }

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

    cocos2d::CCSize getWorldViewSize() {
        auto dgl = utils::getDrawGridLayer();
        if (!dgl) return {};

        return dgl->getCustom()->getWorldViewSize();
    }

    float getOverdrawFactor() {
        auto dgl = utils::getDrawGridLayer();
        if (!dgl) return 0;

        return dgl->getCustom()->getOverdrawFactor();
    }

    bool isDirty() {
        auto dgl = utils::getDrawGridLayer();
        if (!dgl) return false;

        return dgl->getCustom()->isDirty();
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
}