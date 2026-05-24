#include "DrawGridLayer.hpp"
#include "../include/DrawLayers.hpp"

DrawHandler::DrawHandler(DrawGridLayer* drawGridLayer) {
    m_drawGridLayer = drawGridLayer;
    m_shader = CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor);
    
    if (Loader::get()->isModLoaded("raydeeux.grandeditorextension") || Mod::get()->getSettingValue<bool>("extension-override")) {
        m_gridWidthMax = FLT_MAX;
    }
}

void DrawHandler::ensureViewTransformValid() {
    if (!m_dirtyViewTransform && m_drawGridLayer->m_editorLayer->m_playbackMode != PlaybackMode::Playing) return;
    
    const CCSize winSize = CCDirector::get()->getWinSize();
    const float scale = m_drawGridLayer->m_editorLayer->m_objectLayer->getScale();
    const float rotationRad = CC_DEGREES_TO_RADIANS(m_drawGridLayer->m_editorLayer->m_gameState.m_cameraAngle);
    const float sinRot = std::sin(rotationRad);
    const float cosRot = std::cos(rotationRad);
    
    m_cachedOverdrawFactor = std::max(
        winSize.width / winSize.height,
        std::abs(sinRot) + std::abs(cosRot) * 2.f
    );
    
    const CCSize scaledWin = winSize / scale;
    m_cachedWorldViewSize = scaledWin * m_cachedOverdrawFactor;
    m_dirtyViewTransform = false;
}

void DrawHandler::draw() {
    if (m_vanillaDraw) return m_drawGridLayer->draw();
    if (m_drawGridLayer->m_editorLayer->m_objectLayer->getScale() == 0) return;

    GLint oldSrc, oldDst;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &oldSrc);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &oldDst);

    ensureViewTransformValid();
    
    m_hideInvisible = GameManager::get()->getGameVariable("0121");
    const auto& objectLayer = m_drawGridLayer->m_editorLayer->m_objectLayer;
    const auto& gameState = m_drawGridLayer->m_editorLayer->m_gameState;
    const auto& levelSettings = m_drawGridLayer->m_editorLayer->m_levelSettings;
    
    const CCSize winSize = CCDirector::get()->getWinSize();
    const float scale = objectLayer->getScale();
    const CCPoint cameraPos = -objectLayer->getPosition() / scale;
    
    const float height = (levelSettings->m_dynamicLevelHeight ? m_gridHeightMax : MAX_HEIGHT);
    const float halfWorldWidth = m_cachedWorldViewSize.width * 0.5f;
    const float halfWorldHeight = m_cachedWorldViewSize.height * 0.5f;
    
    const float visibleMinX = std::max(cameraPos.x - halfWorldWidth - PADDING, m_gridWidthMin);
    const float visibleMaxX = std::min(cameraPos.x + halfWorldWidth + PADDING, m_gridWidthMax);
    const float visibleMinY = std::max(cameraPos.y - halfWorldHeight - PADDING, m_gridHeightMin);
    const float visibleMaxY = std::min(cameraPos.y + halfWorldHeight + PADDING, height);
    
    if (m_drawGridLayer->m_bReorderChildDirty) {
        m_drawGridLayer->sortAllChildren();
        m_drawGridLayer->m_bReorderChildDirty = false;
    }
    for (auto child : m_drawGridLayer->getChildrenExt()) {
        if (!child->isVisible()) continue;
        if (auto base = typeinfo_cast<DrawGridBase*>(child)) {
            base->draw(visibleMinX, visibleMaxX, visibleMinY, visibleMaxY);
        }
    }

    auto drawBatch = [&](auto& batch, GLenum src, GLenum dst) {
        ccGLBlendFunc(src, dst);
    
        glVertexAttribPointer(
            kCCVertexAttrib_Position,
            2,
            GL_FLOAT,
            GL_FALSE,
            sizeof(good_grid::Vertex),
            &batch[0].position
        );

        glVertexAttribPointer(
            kCCVertexAttrib_Color,
            4,
            GL_UNSIGNED_BYTE,
            GL_TRUE,
            sizeof(good_grid::Vertex),
            &batch[0].color
        );

        glDrawArrays(GL_TRIANGLES, 0, batch.size());
    };

    glEnableVertexAttribArray(kCCVertexAttrib_Position);
    glEnableVertexAttribArray(kCCVertexAttrib_Color);
    m_shader->use();
    m_shader->setUniformsForBuiltins();

    drawBatch(m_lineBatches, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawBatch(m_blendedLineBatches, GL_ONE, GL_ONE);
    drawBatch(m_invertedLineBatches, GL_ONE_MINUS_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);

    m_lineBatches.clear();
    m_blendedLineBatches.clear();
    m_invertedLineBatches.clear();

    ccGLBlendFunc(oldSrc, oldDst);
}

std::vector<good_grid::Vertex>& DrawHandler::batchForMode(DrawGridBase::BlendMode mode) {
    switch (mode) {
        case DrawGridBase::BlendMode::ADDITIVE:
            return m_lineBatches;
        case DrawGridBase::BlendMode::MULTIPLY:
            return m_blendedLineBatches;
        case DrawGridBase::BlendMode::INVERT:
            return m_invertedLineBatches;
    }
}

void DrawHandler::generateTimeMarkers() {
    m_timeMarkers.clear();
    auto markers = CCArrayExt<CCString*>(m_drawGridLayer->m_timeMarkers);
    if (markers.size() < 2) return;

    for (size_t i = 0; i + 1 < markers.size(); i += 2) {
        float pos = numFromString<float>(markers[i]->getCString()).unwrapOrDefault();
        float type = numFromString<float>(markers[i + 1]->getCString()).unwrapOrDefault();
        ccColor4B color;

        static const auto colorA = ccColor4B{255, 255, 0, 255};
        static const auto colorB = ccColor4B{127, 255, 0, 255};
        static const auto colorC = ccColor4B{255, 127, 0, 255};
        static const auto colorD = ccColor4B{0, 0, 0, 0};

        if (type == 0.9f) color = colorA;
        else if (type == 1.0f) color = colorB;
        else if (type >= 0.8f || type == 0.0f) color = colorC;
        else color = colorD;

        m_timeMarkers[pos] = color;
    }
}

const std::unordered_map<float, cocos2d::ccColor4B>& DrawHandler::getTimeMarkers() { 
    return m_timeMarkers; 
}

void DrawHandler::markDirty() { 
    m_dirtyViewTransform = true; 
}

void DrawHandler::setVanillaDraw(bool enabled) { 
    m_vanillaDraw = enabled; 
}

void DrawHandler::overrideGridBoundsSize(cocos2d::CCSize size) { 
    m_gridWidthMax = size.width; 
    m_gridHeightMax = size.height; 
}

void DrawHandler::overrideGridBoundsOrigin(cocos2d::CCPoint point) { 
    m_gridWidthMin = point.x; 
    m_gridHeightMin = point.y; 
}

cocos2d::CCSize DrawHandler::getGridBoundsSize() { 
    return {m_gridWidthMax, m_gridHeightMax}; 
}

cocos2d::CCPoint DrawHandler::getGridBoundsOrigin() { 
    return {m_gridWidthMin, m_gridHeightMin}; 
}

cocos2d::CCSize DrawHandler::getWorldViewSize() { 
    return m_cachedWorldViewSize; 
}

float DrawHandler::getOverdrawFactor() { 
    return m_cachedOverdrawFactor; 
}

bool DrawHandler::isDirty() { 
    return m_dirtyViewTransform; 
}

bool DrawHandler::isVanillaDraw() { 
    return m_vanillaDraw; 
}

bool DrawHandler::isObjectVisible(GameObject* object) {
    bool isHidden = (object->m_isHide && m_hideInvisible) || object->m_isGroupDisabled || object->m_isInvisible;
    return !isHidden || object->m_isSelected;
}

template<class T>
void addChild(DrawGridLayer* dgl, int zOrder) {
    auto child = T::create();
    child->setZOrder(zOrder);
    dgl->addChild(child);
}

DrawGridLayer* MyDrawGridLayer::create(cocos2d::CCNode* p0, LevelEditorLayer* p1) {
	auto ret = DrawGridLayer::create(p0, p1);
	auto fields = static_cast<MyDrawGridLayer*>(ret)->m_fields.self();

	fields->m_customDgl = std::make_shared<DrawHandler>(ret);

    addChild<Grid>(ret, 0);
    addChild<Bounds>(ret, 10);
    addChild<Ground>(ret, 20);
    addChild<GuideObjects>(ret, 30);
    addChild<PreviewLockLine>(ret, 40);
    addChild<EffectLines>(ret, 50);
    addChild<DurationLines>(ret, 60);
    addChild<Guidelines>(ret, 70);
    addChild<BPMTriggers>(ret, 80);
    addChild<PositionLines>(ret, 100);
    addChild<AudioLine>(ret, 90);

	return ret;
}

void MyDrawGridLayer::loadTimeMarkers(gd::string p0) {
	DrawGridLayer::loadTimeMarkers(p0);
	m_fields->m_customDgl->generateTimeMarkers();
}

void MyDrawGridLayer::draw() {
	m_fields->m_customDgl->draw();
}

DrawHandler* MyDrawGridLayer::getCustom() {
	return m_fields->m_customDgl.get();
}

void MyDrawGridLayer::markDirty() {
	m_fields->m_customDgl->markDirty();
}

void MyEditorUI::updateZoom(float p0) {
    EditorUI::updateZoom(p0);
    static_cast<MyDrawGridLayer*>(m_editorLayer->m_drawGridLayer)->markDirty();
}