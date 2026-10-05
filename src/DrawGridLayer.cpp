#include "DrawGridLayer.hpp"
#include "../include/DrawLayers.hpp"

namespace alpha::grid {

DrawHandler::Batch::Batch(ccBlendFunc func) {
    m_blendFunc = func;
}

void DrawHandler::Batch::draw() const {
    ccGLBlendFunc(m_blendFunc.src, m_blendFunc.dst);
    
    glVertexAttribPointer(
        kCCVertexAttrib_Position,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(alpha::grid::Vertex),
        &m_batch[0].position
    );

    glVertexAttribPointer(
        kCCVertexAttrib_Color,
        4,
        GL_UNSIGNED_BYTE,
        GL_TRUE,
        sizeof(alpha::grid::Vertex),
        &m_batch[0].color
    );

    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(alpha::grid::Vertex),
        &m_batch[0].uv
    );

    glVertexAttribPointer(
        3,
        1,
        GL_FLOAT,
        GL_FALSE,
        sizeof(alpha::grid::Vertex),
        &m_batch[0].angle
    );

    glDrawArrays(GL_TRIANGLES, 0, m_batch.size());
}

DrawHandler::DrawHandler(DrawGridLayer* drawGridLayer) {
    m_drawGridLayer = drawGridLayer;

    auto vert = R"(
        attribute vec4 a_position;
        attribute vec4 a_color;
        attribute vec2 a_uv;
        attribute float a_angle;

        varying vec4 v_color;
        varying vec2 v_uv;
        varying float v_angle;

        void main() {
            gl_Position = CC_MVPMatrix * a_position;
            v_color = a_color;
            v_uv = a_uv;
            v_angle = a_angle;
        }
    )";

    auto frag = R"(
        #ifdef GL_ES
        precision mediump float;
        #endif

        varying vec4 v_color;
        varying vec2 v_uv;
        varying float v_angle;

        void main() {
            vec2 derivative = fwidth(v_uv);

            vec2 distToBottomLeft = v_uv / derivative;
            vec2 distToTopRight = (vec2(1.0) - v_uv) / derivative;

            float minEdgeDistX = min(distToBottomLeft.x, distToTopRight.x);
            float minEdgeDistY = min(distToBottomLeft.y, distToTopRight.y);

            float edgeDist = min(minEdgeDistX, minEdgeDistY);

            float axisAngle = mod(abs(v_angle), 1.5707963);
            float deviation = min(axisAngle, 1.5707963 - axisAngle);

            float aa = smoothstep(0.0, 0.0001, deviation);
            float alpha = mix(1.0, smoothstep(0.0, 1.0, edgeDist), aa);

            gl_FragColor = vec4(v_color.rgb * alpha, v_color.a * alpha);
        }
    )";

    m_shader = new CCGLProgram();
    m_shader->autorelease();
    m_shader->initWithVertexShaderByteArray(vert, frag);
    m_shader->addAttribute("a_position", kCCVertexAttrib_Position);
    m_shader->addAttribute("a_color", kCCVertexAttrib_Color);
    m_shader->addAttribute("a_uv", 2);
    m_shader->addAttribute("a_angle", 3);
    m_shader->link();
    m_shader->updateUniforms();
    
    if (Loader::get()->isModLoaded("raydeeux.grandeditorextension") || Mod::get()->getSettingValue<bool>("extension-override")) {
        m_gridWidthMax = FLT_MAX;
    }
}

void DrawHandler::draw() {
    if (m_vanillaDraw) return m_drawGridLayer->draw();
    if (m_drawGridLayer->m_editorLayer->m_objectLayer->getScale() == 0.f) return;

    GLint oldSrc, oldDst;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &oldSrc);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &oldDst);

    m_hideInvisible = GameManager::get()->getGameVariable(GameVar::HideInvisible);

    auto& gameState = m_drawGridLayer->m_editorLayer->m_gameState;
    auto objectLayer = m_drawGridLayer->m_editorLayer->m_objectLayer;
    auto levelSettings = m_drawGridLayer->m_editorLayer->m_levelSettings;
    
    float scale = objectLayer->getScale();
    auto winSize = CCDirector::get()->getWinSize();
    auto cameraPos = -objectLayer->getPosition() / scale;

    auto scaledWin = winSize / scale;

    auto rad = CC_DEGREES_TO_RADIANS(m_drawGridLayer->m_editorLayer->m_gameState.m_cameraAngle);
    m_sin = std::sin(rad);
    m_cos = std::cos(rad);

    auto visibleSize = CCSize{scaledWin.width * std::abs(m_cos) + scaledWin.height * std::abs(m_sin), scaledWin.width * std::abs(m_sin) + scaledWin.height * std::abs(m_cos)};

    float height = levelSettings->m_dynamicLevelHeight ? m_gridHeightMax : DrawGridBase::MaxHeight;
    if (m_drawGridLayer->m_editorLayer->m_gameState.m_cameraAngle != 0.f) {
        cameraPos -= visibleSize / 2.f;
    }

    float visibleMinX = std::max(cameraPos.x, m_gridWidthMin);
    float visibleMaxX = std::min(cameraPos.x + visibleSize.width, m_gridWidthMax);
    float visibleMinY = std::max(cameraPos.y, m_gridHeightMin);
    float visibleMaxY = std::min(cameraPos.y + visibleSize.height, height);
    
    auto visibleRect = CCRect{
        visibleMinX, 
        visibleMinY, 
        visibleMaxX - visibleMinX, 
        visibleMaxY - visibleMinY
    };

    if (m_drawGridLayer->m_bReorderChildDirty) {
        m_drawGridLayer->sortAllChildren();
        m_drawGridLayer->m_bReorderChildDirty = false;
    }

    for (auto child : m_drawGridLayer->getChildrenExt()) {
        if (!child->isVisible()) continue;

        if (auto base = typeinfo_cast<DrawGridBase*>(child)) {
            base->draw(visibleRect);
        }
    }

    glEnableVertexAttribArray(kCCVertexAttrib_Position);
    glEnableVertexAttribArray(kCCVertexAttrib_Color);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    m_shader->use();
    m_shader->setUniformsForBuiltins();

    for (const auto& batch : m_batches) {
        batch.draw();
    }

    m_batches.clear();
    m_activeBatch = nullptr;

    ccGLBlendFunc(oldSrc, oldDst);
}

std::vector<alpha::grid::Vertex>& DrawHandler::batchForFunc(ccBlendFunc func) {
    if (m_activeBatch && m_activeBatch->m_blendFunc.src == func.src && m_activeBatch->m_blendFunc.dst == func.dst) {
        return m_activeBatch->m_batch;
    }

    auto& batch = m_batches.emplace_back(func);
    m_activeBatch = &batch;

    return m_activeBatch->m_batch;
}

void DrawHandler::generateTimeMarkers() {
    m_timeMarkers.clear();
    auto markers = CCArrayExt<CCString*>(m_drawGridLayer->m_timeMarkers);
    if (markers.size() < 2) return;

    for (size_t i = 0; i + 1 < markers.size(); i += 2) {
        float pos = numFromString<float>(markers[i]->getCString()).unwrapOrDefault();
        float type = numFromString<float>(markers[i + 1]->getCString()).unwrapOrDefault();

        ccColor4B color;

        if (type == 0.9f) m_timeMarkers.try_emplace(pos, getColorA());
        else if (type == 1.f) m_timeMarkers.try_emplace(pos, getColorB());
        else if (type >= 0.8f || type == 0.f) m_timeMarkers.try_emplace(pos, getColorC());
        else m_timeMarkers.try_emplace(pos, getColorD());
    }
}

const Color& DrawHandler::getColorA() {
    static Color defaultColor = {255, 255, 0, 255};
    return defaultColor;
}

const Color& DrawHandler::getColorB() {
    static Color defaultColor = {127, 255, 0, 255};
    return defaultColor;
}

const Color& DrawHandler::getColorC() {
    static Color defaultColor = {255, 127, 0, 255};
    return defaultColor;
}

const Color& DrawHandler::getColorD() {
    static Color defaultColor = {0, 0, 0, 0};
    return defaultColor;
}

const std::unordered_map<float, const Color&>& DrawHandler::getTimeMarkers() { 
    return m_timeMarkers; 
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

bool DrawHandler::isVanillaDraw() { 
    return m_vanillaDraw; 
}

bool DrawHandler::isObjectVisible(GameObject* object) {
    bool isHidden = (object->m_isHide && m_hideInvisible) || object->m_isGroupDisabled || object->m_isInvisible;
    return !isHidden || object->m_isSelected;
}

float DrawHandler::getSin() {
    return m_sin;
}

float DrawHandler::getCos() {
    return m_cos;
}

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

	fields->m_customDgl = std::make_shared<alpha::grid::DrawHandler>(ret);

    addChild<alpha::grid::Grid>(ret, 0);
    addChild<alpha::grid::Bounds>(ret, 100);
    addChild<alpha::grid::Ground>(ret, 200);
    addChild<alpha::grid::GuideObjects>(ret, 300);
    addChild<alpha::grid::PreviewLockLine>(ret, 400);
    addChild<alpha::grid::EffectLines>(ret, 500);
    addChild<alpha::grid::DurationLines>(ret, 600);
    addChild<alpha::grid::Guidelines>(ret, 700);
    addChild<alpha::grid::BPMTriggers>(ret, 800);
    addChild<alpha::grid::PositionLines>(ret, 900);
    addChild<alpha::grid::AudioLine>(ret, 1000);

	return ret;
}

void MyDrawGridLayer::loadTimeMarkers(gd::string p0) {
	DrawGridLayer::loadTimeMarkers(p0);
	m_fields->m_customDgl->generateTimeMarkers();
}

void MyDrawGridLayer::draw() {
	m_fields->m_customDgl->draw();
}

alpha::grid::DrawHandler* MyDrawGridLayer::getCustom() {
	return m_fields->m_customDgl.get();
}

void MyEditorUI::ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event) {
    bool willSnap = false;

    if (m_snapObjectExists && m_continuousSnap && m_snapObject) {
        bool is90 = static_cast<int>(m_snapObject->getRotation()) % 90 == 0;
        auto world = getTouchPoint(touch, event);

        if (is90 && !world.equals(m_swipeStart) && GameManager::get()->getGameVariable(GameVar::EnableSnap)) {
            willSnap = true;
        }
    }

    EditorUI::ccTouchEnded(touch, event);

    if (willSnap) {
        m_editorLayer->m_drawGridLayer->m_updateSpeedObjects = true;
    }
}