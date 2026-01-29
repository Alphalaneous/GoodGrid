#include <Geode/Geode.hpp>
#include <Geode/modify/DrawGridLayer.hpp>
#include "../include/DrawGridAPI.hpp"

using namespace geode::prelude;

class $modify(MyDrawGridLayer, DrawGridLayer) {

	static void onModify(auto& self) {
        (void) self.setHookPriority("DrawGridLayer::draw", Priority::Replace);
    }

    static DrawGridLayer* create(cocos2d::CCNode* p0, LevelEditorLayer* p1) {
		auto ret = DrawGridLayer::create(p0, p1);
		DrawGridAPI::get().init(ret, CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
		return ret;
	}

    void loadTimeMarkers(gd::string p0) {
		DrawGridLayer::loadTimeMarkers(p0);
		DrawGridAPI::get().generateTimeMarkers();
	}

    void draw() {
		DrawGridAPI::get().draw();
	}
};