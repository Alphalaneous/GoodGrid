#pragma once

#include <Geode/Geode.hpp>
#include "../include/DrawGridBase.hpp"
#include <Geode/modify/EditorUI.hpp>
#include <Geode/modify/DrawGridLayer.hpp>
#include "Vertex.hpp"

using namespace geode::prelude;
using namespace good_grid;

class DrawHandler {
public:
    DrawHandler(DrawGridLayer* drawGridLayer);

    void draw();

    std::vector<good_grid::Vertex>& batchForMode(DrawGridBase::DrawMode mode);
    
    void generateTimeMarkers();
    const std::unordered_map<float, cocos2d::ccColor4B>& getTimeMarkers();
    void markDirty();

    void setVanillaDraw(bool enabled);

    void overrideGridBoundsSize(cocos2d::CCSize size);
    void overrideGridBoundsOrigin(cocos2d::CCPoint point);

    cocos2d::CCSize getGridBoundsSize();
    cocos2d::CCPoint getGridBoundsOrigin();

    cocos2d::CCSize getWorldViewSize();
    float getOverdrawFactor();

    bool isDirty();
    bool isVanillaDraw();

    bool isObjectVisible(GameObject* object);

    static constexpr float MAX_HEIGHT = 2490.f;
    static constexpr float GROUND_OFFSET = 90.f;
    static constexpr float PADDING = 500.f;
protected:
    void ensureViewTransformValid();

    std::vector<good_grid::Vertex> m_lineBatches;
    std::vector<good_grid::Vertex> m_blendedLineBatches;
    std::vector<good_grid::Vertex> m_invertedLineBatches;

    DrawGridLayer* m_drawGridLayer = nullptr;
    CCGLProgram* m_shader = nullptr;

    bool m_vanillaDraw = false;
    bool m_dirtyViewTransform = true;

    float m_gridWidthMin = -3000.f;
    float m_gridHeightMin = -3000.f;

    float m_gridWidthMax = 240000.f;
    float m_gridHeightMax = 30090.f;

    std::unordered_map<float, cocos2d::ccColor4B> m_timeMarkers;

    bool m_hideInvisible;

    CCSize m_cachedWorldViewSize;
    float m_cachedOverdrawFactor = 1.f;
};

class $modify(MyDrawGridLayer, DrawGridLayer) {
	static void onModify(auto& self) {
        (void) self.setHookPriority("DrawGridLayer::draw", Priority::Replace);
    }

	struct Fields {
		std::shared_ptr<DrawHandler> m_customDgl;
	};

    static DrawGridLayer* create(cocos2d::CCNode* p0, LevelEditorLayer* p1);

    void loadTimeMarkers(gd::string p0);
    void draw();
    void markDirty();

    DrawHandler* getCustom();
};

class $modify(MyEditorUI, EditorUI) {
	void updateZoom(float p0);
};