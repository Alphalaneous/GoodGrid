#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/DrawGridLayer.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "Vertex.hpp"

using namespace geode::prelude;

namespace alpha::grid {

class DrawHandler {
public:
    struct Batch {
        Batch(ccBlendFunc mode);

        std::vector<alpha::grid::Vertex> m_batch;
        ccBlendFunc m_blendFunc;

        void draw() const;
    };

    DrawHandler(DrawGridLayer* drawGridLayer);

    void draw();

    std::vector<alpha::grid::Vertex>& batchForFunc(ccBlendFunc func);
    
    void generateTimeMarkers();
    const std::unordered_map<float, cocos2d::ccColor4B>& getTimeMarkers();

    void setVanillaDraw(bool enabled);

    void overrideGridBoundsSize(cocos2d::CCSize size);
    void overrideGridBoundsOrigin(cocos2d::CCPoint point);

    cocos2d::CCSize getGridBoundsSize();
    cocos2d::CCPoint getGridBoundsOrigin();

    bool isVanillaDraw();
    bool isObjectVisible(GameObject* object);

    float getSin();
    float getCos();

    static constexpr float MAX_HEIGHT = 2490.f;
    static constexpr float GROUND_OFFSET = 90.f;

protected:
    std::vector<Batch> m_batches;
    Batch* m_activeBatch = nullptr;
    DrawGridLayer* m_drawGridLayer = nullptr;
    Ref<CCGLProgram> m_shader = nullptr;

    bool m_vanillaDraw = false;

    float m_gridWidthMin = -3000.f;
    float m_gridHeightMin = -3000.f;

    float m_gridWidthMax = 240000.f;
    float m_gridHeightMax = 30090.f;

    std::unordered_map<float, cocos2d::ccColor4B> m_timeMarkers;

    bool m_hideInvisible;

    float m_sin = 0.f;
    float m_cos = 0.f;
};

}

class $modify(MyDrawGridLayer, DrawGridLayer) {
	static void onModify(auto& self) {
        (void) self.setHookPriority("DrawGridLayer::draw", Priority::Replace);
    }

	struct Fields {
		std::shared_ptr<alpha::grid::DrawHandler> m_customDgl;
	};

    static DrawGridLayer* create(cocos2d::CCNode* p0, LevelEditorLayer* p1);

    void loadTimeMarkers(gd::string p0);
    void draw();

    alpha::grid::DrawHandler* getCustom();
};

class $modify(MyEditorUI, EditorUI) {
    void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
};