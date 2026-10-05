#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/DrawGridLayer.hpp>
#include <Geode/modify/EditorUI.hpp>
#include "Vertex.hpp"
#include "../include/DrawGridBase.hpp"

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
    const std::unordered_map<float, const Color&>& getTimeMarkers();

    void setVanillaDraw(bool enabled);

    void overrideGridBoundsSize(cocos2d::CCSize size);
    void overrideGridBoundsOrigin(cocos2d::CCPoint point);

    cocos2d::CCSize getGridBoundsSize();
    cocos2d::CCPoint getGridBoundsOrigin();

    bool isVanillaDraw();
    bool isObjectVisible(GameObject* object);

    float getSin();
    float getCos();

    static const Color& getColorA();
    static const Color& getColorB();
    static const Color& getColorC();
    static const Color& getColorD();

protected:
    std::vector<Batch> m_batches;
    Batch* m_activeBatch = nullptr;
    DrawGridLayer* m_drawGridLayer = nullptr;
    Ref<CCGLProgram> m_shader = nullptr;

    bool m_vanillaDraw = false;

    float m_gridWidthMin = DrawGridBase::MinWidth;
    float m_gridHeightMin = DrawGridBase::MinHeight;

    float m_gridWidthMax = DrawGridBase::MaxWidth;
    float m_gridHeightMax = DrawGridBase::MaxDynamicHeight;

    std::unordered_map<float, const Color&> m_timeMarkers;

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