#include "../include/DrawGridAPI.hpp"
#include "../include/DrawLayers.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

const float MAX_HEIGHT = 2490.f;
const float GROUND_OFFSET = 90.f;
const float PADDING = 500.f;

LineColor::LineColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
    m_colorA = ccColor4B{r, g, b, a};
}

LineColor::LineColor(const cocos2d::ccColor4B& colorA) {
    m_colorA = colorA;
}

LineColor::LineColor(const cocos2d::ccColor4B& colorA, const cocos2d::ccColor4B& colorB) {
    m_colorA = colorA;
    m_colorB = colorB;
    m_hasColorB = true;
}

cocos2d::ccColor4B LineColor::getColorA() const {
    return m_colorA;
}

cocos2d::ccColor4B LineColor::getColorB() const {
    return m_hasColorB ? m_colorB : m_colorA;
}

struct DrawGridAPIImpl {
    float m_gridWidthMin = -3000.f;
    float m_gridHeightMin = -3000.f;
    float m_gridWidthMax = 240000.f;
    float m_gridHeightMax = 30090.f;
    bool m_vanillaDraw = false;
    bool m_hideInvisible = false;
    bool m_lineSmoothing = false;
    float m_lineSmoothingLimit = 0.4f;
    bool m_dirtyViewTransform = true;
    bool m_shouldSort = true;
    float m_cachedOverdrawFactor = 1.f;
    CCSize m_cachedWorldViewSize;
    CCGLProgram* m_shader = nullptr;
    std::unordered_map<float, ccColor4B> m_timeMarkers;
    std::map<float, std::vector<Vertex>> m_lineVertsBuffer;
    std::map<float, std::vector<Vertex>> m_blendedLineVertsBuffer;
    std::vector<Vertex> m_rectVertsBuffer;
    std::vector<Vertex> m_blendedRectVertsBuffer;
    std::vector<Vertex> m_rectOutlineVertsBuffer;
    std::vector<Vertex> m_blendedRectOutlineVertsBuffer;
    DrawGridLayer* m_drawGridLayer = nullptr;
    std::vector<std::unique_ptr<DrawNode>> m_drawNodes;
};

DrawGridAPI::DrawGridAPI() : m_impl(std::make_unique<DrawGridAPIImpl>()) {
    addDraw<Grid>("grid");
    addDraw<Bounds>("bounds");
    addDraw<Ground>("ground");
    addDraw<GuideObjects>("guide-objects");
    addDraw<PreviewLockLine>("preview-lock-line");
    addDraw<EffectLines>("effect-lines");
    addDraw<DurationLines>("duration-lines");
    addDraw<Guidelines>("guidelines");
    addDraw<BPMTriggers>("bpm-triggers");
    addDraw<AudioLine>("audio-line");
    addDraw<PositionLines>("position-lines");
}

DrawGridAPI::~DrawGridAPI() = default;

DrawGridAPI::DrawGridAPI(DrawGridAPI&&) noexcept = default;

DrawGridAPI& DrawGridAPI::operator=(DrawGridAPI&&) noexcept = default;

DrawGridAPI& DrawGridAPI::get() {
    static DrawGridAPI instance;
    return instance;
}

void DrawGridAPI::init(DrawGridLayer* drawGridLayer, cocos2d::CCGLProgram* shader) {
    m_impl->m_drawGridLayer = drawGridLayer;
    m_impl->m_shader = shader;
    m_impl->m_dirtyViewTransform = true;
    m_impl->m_cachedOverdrawFactor = 1.f;
    m_impl->m_cachedWorldViewSize = CCSize{0, 0};
    m_impl->m_shouldSort = true;
    m_impl->m_lineVertsBuffer.clear();
    m_impl->m_blendedLineVertsBuffer.clear();
    m_impl->m_rectVertsBuffer.clear();
    m_impl->m_blendedRectVertsBuffer.clear();
    m_impl->m_rectOutlineVertsBuffer.clear();
    m_impl->m_blendedRectOutlineVertsBuffer.clear();

    if (Loader::get()->isModLoaded("raydeeux.grandeditorextension") || Mod::get()->getSettingValue<bool>("extension-override")) {
        m_impl->m_gridWidthMax = FLT_MAX;
    }

    for (const auto& drawNode : m_impl->m_drawNodes) {
        if (drawNode->isEnabled()) {
            drawNode->init(m_impl->m_drawGridLayer);
        }
    }
}

void DrawGridAPI::markDirty() { 
    m_impl->m_dirtyViewTransform = true; 
}

void DrawGridAPI::shouldSort() {
    m_impl->m_shouldSort = true;
}

void DrawGridAPI::sort() {
    std::sort(m_impl->m_drawNodes.begin(), m_impl->m_drawNodes.end(), [](const std::unique_ptr<DrawNode>& a, const std::unique_ptr<DrawNode>& b) {
        return a->getZOrder() < b->getZOrder();
    });

    m_impl->m_shouldSort = false;
}

void DrawGridAPI::setVanillaDraw(bool enabled) { 
    m_impl->m_vanillaDraw = enabled; 
}

void DrawGridAPI::setHideInvisible(bool enabled) { 
    m_impl->m_hideInvisible = enabled; 
}

void DrawGridAPI::setLineSmoothing(bool enabled) { 
    m_impl->m_lineSmoothing = enabled; 
}

void DrawGridAPI::setLineSmoothingLimit(float limit) { 
    m_impl->m_lineSmoothingLimit = limit; 
}

void DrawGridAPI::overrideGridBoundsSize(cocos2d::CCSize size) { 
    m_impl->m_gridWidthMax = size.width; 
    m_impl->m_gridHeightMax = size.height; 
}

void DrawGridAPI::overrideGridBoundsOrigin(cocos2d::CCPoint point) { 
    m_impl->m_gridWidthMin = point.x; 
    m_impl->m_gridHeightMin = point.y; 
}

cocos2d::CCSize DrawGridAPI::getGridBoundsSize() { 
    return {m_impl->m_gridWidthMax, m_impl->m_gridHeightMax}; 
}

cocos2d::CCPoint DrawGridAPI::getGridBoundsOrigin() { 
    return {m_impl->m_gridWidthMin, m_impl->m_gridHeightMin}; 
}

void DrawGridAPI::generateTimeMarkers() {
    m_impl->m_timeMarkers.clear();
    auto markers = CCArrayExt<CCString*>(m_impl->m_drawGridLayer->m_timeMarkers);
    if (markers.size() < 2) return;

    for (size_t i = 0; i + 1 < markers.size(); i += 2) {
        float pos = numFromString<float>(markers[i]->getCString()).unwrapOrDefault();
        float type = numFromString<float>(markers[i + 1]->getCString()).unwrapOrDefault();
        ccColor4B color;

        if (type == 0.9f) color = ccColor4B{127, 255, 0, 255};
        else if (type == 1.0f) color = ccColor4B{255, 255, 0, 255};
        else if (type >= 0.8f || type == 0.0f) color = ccColor4B{255, 127, 0, 255};
        else color = ccColor4B{0, 0, 0, 0};

        m_impl->m_timeMarkers[pos] = color;
    }
}

std::unordered_map<float, cocos2d::ccColor4B> DrawGridAPI::getTimeMarkers() { 
    return m_impl->m_timeMarkers; 
}

cocos2d::CCSize DrawGridAPI::getWorldViewSize() { 
    return m_impl->m_cachedWorldViewSize; 
}

float DrawGridAPI::getOverdrawFactor() { 
    return m_impl->m_cachedOverdrawFactor; 
}

bool DrawGridAPI::isDirty() { 
    return m_impl->m_dirtyViewTransform; 
}

bool DrawGridAPI::isVanillaDraw() { 
    return m_impl->m_vanillaDraw; 
}

bool DrawGridAPI::hasLineSmoothing() {
    return m_impl->m_lineSmoothing;
}

float DrawGridAPI::getLineSmoothingLimit() {
    return m_impl->m_lineSmoothingLimit;
}

void DrawGridAPI::ensureViewTransformValid() {
    if (!m_impl->m_dirtyViewTransform) return;
    
    const CCSize winSize = CCDirector::get()->getWinSize();
    const float scale = m_impl->m_drawGridLayer->m_editorLayer->m_objectLayer->getScale();
    const float rotationRad = CC_DEGREES_TO_RADIANS(m_impl->m_drawGridLayer->m_editorLayer->m_gameState.m_cameraAngle);
    const float sinRot = std::sin(rotationRad);
    const float cosRot = std::cos(rotationRad);
    
    m_impl->m_cachedOverdrawFactor = std::max(
        winSize.width / winSize.height,
        std::abs(sinRot) + std::abs(cosRot) * 2.f
    );
    
    const CCSize scaledWin = winSize / scale;
    m_impl->m_cachedWorldViewSize = scaledWin * m_impl->m_cachedOverdrawFactor;
    m_impl->m_dirtyViewTransform = false;
}

cocos2d::CCPoint DrawGridAPI::getPortalMinMax(GameObject* obj) {
    GameObjectType objectType = obj->m_objectType;
    static constexpr float defaultHeight = 300.0f;
    static constexpr float ballPortalHeight = 240.0f;
    static constexpr float spiderPortalHeight = 270.0f;
    static constexpr float gridStep = 30.0f;
    static constexpr float minYClamp = 90.0f;
    
    float height = defaultHeight;
    switch (obj->m_objectType) {
        case GameObjectType::BallPortal: height = ballPortalHeight; break;
        case GameObjectType::SpiderPortal: height = spiderPortalHeight; break;
        default: break;
    }
    
    float posY = obj->getPositionY();
    float yMin = std::floor((posY - height * 0.5f) / gridStep) * gridStep;
    if (yMin < minYClamp) yMin = minYClamp;
    return CCPoint{ yMin, yMin + height };
}

template <typename T>
void reserveIfNeeded(std::vector<T>& vec, size_t minCapacity) {
    if (vec.capacity() < minCapacity) vec.reserve(minCapacity);
}

/*
    here's where the performance happens, it's all because I cheat! The editor doesn't have many different
    width lines, so I draw them separately in their own batches reducing the amount of gl draw calls.
    We don't really have to care about depth and ordering anyways, since a majority of the lines are 1px
    and will be ordered properly. Wider widths are rendered later, but there are few cases of that, and it
    likely would not yield problems, if it does though for one of your own mods, then it is recommended 
    to create your own layer to draw on as this layer is now specially made with performance in mind.
*/ 
void DrawGridAPI::batchDraw() {
    bool shouldSmooth = m_impl->m_drawGridLayer->m_editorLayer->m_objectLayer->getScale() >= m_impl->m_lineSmoothingLimit && m_impl->m_lineSmoothing;
    float widthModifier = 0;

    constexpr size_t kReserveLines = 4096;
    constexpr size_t kReserveRects = 2048;

    #ifdef GEODE_IS_DESKTOP
    if (shouldSmooth) {
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        widthModifier += 0.2;
    }
    #endif 
    
    ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    if (!m_impl->m_lineVertsBuffer.empty()){
        for (auto& [k, v] : m_impl->m_lineVertsBuffer) {
            reserveIfNeeded(v, kReserveLines);
            if (!v.empty()) {
                glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &v[0].position);
                glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), &v[0].color);
                float width = k + widthModifier;
                glLineWidth(width);
                glDrawArrays(GL_LINES, 0, v.size());
            }
        }
    }

    if (!m_impl->m_rectVertsBuffer.empty()){
        reserveIfNeeded(m_impl->m_rectVertsBuffer, kReserveRects);
        glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_impl->m_rectVertsBuffer[0].position);
        glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), &m_impl->m_rectVertsBuffer[0].color);
        glDrawArrays(GL_TRIANGLES, 0, m_impl->m_rectVertsBuffer.size());
    }
    
    if (!m_impl->m_rectOutlineVertsBuffer.empty()){
        reserveIfNeeded(m_impl->m_rectOutlineVertsBuffer, kReserveRects);
        glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_impl->m_rectOutlineVertsBuffer[0].position);
        glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), &m_impl->m_rectOutlineVertsBuffer[0].color);
        glDrawArrays(GL_TRIANGLES, 0, m_impl->m_rectOutlineVertsBuffer.size());
    }

    glBlendFunc(GL_ONE, GL_ONE);

    if (!m_impl->m_blendedLineVertsBuffer.empty()){
        for (auto& [k, v] : m_impl->m_blendedLineVertsBuffer) {
            reserveIfNeeded(v, kReserveLines);
            if (!v.empty()) {
                glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &v[0].position);
                glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), &v[0].color);
                float width = k + widthModifier;
                glLineWidth(width);
                glDrawArrays(GL_LINES, 0, v.size());
            }
        }
    }

    if (!m_impl->m_blendedRectVertsBuffer.empty()){
        reserveIfNeeded(m_impl->m_blendedRectVertsBuffer, kReserveRects);
        glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_impl->m_blendedRectVertsBuffer[0].position);
        glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), &m_impl->m_blendedRectVertsBuffer[0].color);
        glDrawArrays(GL_TRIANGLES, 0, m_impl->m_blendedRectVertsBuffer.size());
    }
    
    if (!m_impl->m_blendedRectOutlineVertsBuffer.empty()){
        reserveIfNeeded(m_impl->m_blendedRectOutlineVertsBuffer, kReserveRects);
        glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), &m_impl->m_blendedRectOutlineVertsBuffer[0].position);
        glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), &m_impl->m_blendedRectOutlineVertsBuffer[0].color);
        glDrawArrays(GL_TRIANGLES, 0, m_impl->m_blendedRectOutlineVertsBuffer.size());
    }

    #ifdef GEODE_IS_DESKTOP
    if (shouldSmooth) {
        glDisable(GL_LINE_SMOOTH);
    }
    #endif

    for (auto& [_, v] : m_impl->m_lineVertsBuffer) v.resize(0);
    for (auto& [_, v] : m_impl->m_blendedLineVertsBuffer) v.resize(0);
    m_impl->m_rectVertsBuffer.resize(0);
    m_impl->m_blendedRectVertsBuffer.resize(0);
    m_impl->m_rectOutlineVertsBuffer.resize(0);
    m_impl->m_blendedRectOutlineVertsBuffer.resize(0);
}

void DrawGridAPI::drawLine(const cocos2d::ccVertex2F& a, const cocos2d::ccVertex2F& b, const LineColor& color, float width, bool blend) {
    if (blend) {
        m_impl->m_blendedLineVertsBuffer[width].push_back({a, color.getColorA()});
        m_impl->m_blendedLineVertsBuffer[width].push_back({b, color.getColorB()});
    } else {
        m_impl->m_lineVertsBuffer[width].push_back({a, color.getColorA()});
        m_impl->m_lineVertsBuffer[width].push_back({b, color.getColorB()});
    }
}

std::array<Vertex, 6> DrawGridAPI::rectToTriangles(const CCRect& rect, const ccColor4B& color) {
    float x = rect.getMinX();
    float y = rect.getMinY();
    float w = rect.size.width;
    float h = rect.size.height;

    return {
        Vertex{x,     y, color},
        Vertex{x + w, y, color},
        Vertex{x + w, y + h, color},

        Vertex{x,     y, color},
        Vertex{x + w, y + h, color},
        Vertex{x,     y + h, color}
    };
}

std::array<Vertex, 24> DrawGridAPI::rectToBorderTriangles(const CCRect& rect, const ccColor4B& color, float width) {
    float x = rect.getMinX();
    float y = rect.getMinY();
    float w = rect.size.width;
    float h = rect.size.height;

    float t = width;

    float ox0 = x - t;
    float oy0 = y - t;
    float ox1 = x + w + t;
    float oy1 = y + h + t;

    float ix0 = x;
    float iy0 = y;
    float ix1 = x + w;
    float iy1 = y + h;

    return {
        Vertex{ox0, oy0, color},
        Vertex{ox1, oy0, color},
        Vertex{ix1, iy0, color},
        Vertex{ox0, oy0, color},
        Vertex{ix1, iy0, color},
        Vertex{ix0, iy0, color},

        Vertex{ox1, oy0, color},
        Vertex{ox1, oy1, color},
        Vertex{ix1, iy1, color},
        Vertex{ox1, oy0, color},
        Vertex{ix1, iy1, color},
        Vertex{ix1, iy0, color},

        Vertex{ox1, oy1, color},
        Vertex{ox0, oy1, color},
        Vertex{ix0, iy1, color},
        Vertex{ox1, oy1, color},
        Vertex{ix0, iy1, color},
        Vertex{ix1, iy1, color},

        Vertex{ox0, oy1, color},
        Vertex{ox0, oy0, color},
        Vertex{ix0, iy0, color},
        Vertex{ox0, oy1, color},
        Vertex{ix0, iy0, color},
        Vertex{ix0, iy1, color}
    };
}

void DrawGridAPI::drawRect(const CCRect& rect, const ccColor4B& color, bool blend) {
    if (blend) {
        for (Vertex v : rectToTriangles(rect, color)) {
            m_impl->m_blendedRectVertsBuffer.push_back(v);
        }
    } else {
        for (Vertex v : rectToTriangles(rect, color)) {
            m_impl->m_rectVertsBuffer.push_back(v);
        }
    }
}

void DrawGridAPI::drawRectOutline(const CCRect& rect, const ccColor4B& color, float width, bool blend) {
    if (blend) {
        for (Vertex v : rectToBorderTriangles(rect, color, width)) {
            m_impl->m_blendedRectOutlineVertsBuffer.push_back(v);
        }
    } else {
        for (Vertex v : rectToBorderTriangles(rect, color, width)) {
            m_impl->m_rectOutlineVertsBuffer.push_back(v);
        }
    }
}

bool DrawGridAPI::isObjectVisible(GameObject* object) {
    bool isHidden = (object->m_isHide && m_impl->m_hideInvisible) || object->m_isGroupDisabled || object->m_isInvisible;
    return !isHidden || object->m_isSelected;
}

void DrawGridAPI::addDrawInternal(const std::string& id, std::unique_ptr<DrawNode> drawNode) { 
    drawNode->setID(id);
    m_impl->m_drawNodes.push_back(std::move(drawNode)); 
}

const std::vector<std::unique_ptr<DrawNode>>& DrawGridAPI::getDrawNodes() {
    return m_impl->m_drawNodes;
}

Result<DrawNode&> DrawGridAPI::getNodeByID(const std::string& id) {
    for (const auto& drawNode : m_impl->m_drawNodes) {
        if (drawNode->getID() == id) return geode::Ok(*drawNode);
    }
    return geode::Err("Node not found with ID");
}

void DrawGridAPI::draw() {
    if (m_impl->m_vanillaDraw) return m_impl->m_drawGridLayer->draw();
    if (m_impl->m_drawGridLayer->m_editorLayer->m_objectLayer->getScale() == 0) return;

    GLint oldSrc, oldDst;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &oldSrc);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &oldDst);

    ensureViewTransformValid();
    sort();
    
    m_impl->m_hideInvisible = GameManager::get()->getGameVariable("0121");
    const auto& objectLayer = m_impl->m_drawGridLayer->m_editorLayer->m_objectLayer;
    const auto& gameState = m_impl->m_drawGridLayer->m_editorLayer->m_gameState;
    const auto& levelSettings = m_impl->m_drawGridLayer->m_editorLayer->m_levelSettings;
    
    const CCSize winSize = CCDirector::get()->getWinSize();
    const float scale = objectLayer->getScale();
    const CCPoint cameraPos = -objectLayer->getPosition() / scale;
    
    const float height = (levelSettings->m_dynamicLevelHeight ? m_impl->m_gridHeightMax : MAX_HEIGHT);
    const float halfWorldWidth = m_impl->m_cachedWorldViewSize.width * 0.5f;
    const float halfWorldHeight = m_impl->m_cachedWorldViewSize.height * 0.5f;
    
    const float visibleMinX = std::max(cameraPos.x - halfWorldWidth - PADDING, m_impl->m_gridWidthMin);
    const float visibleMaxX = std::min(cameraPos.x + halfWorldWidth + PADDING, m_impl->m_gridWidthMax);
    const float visibleMinY = std::max(cameraPos.y - halfWorldHeight - PADDING, m_impl->m_gridHeightMin);
    const float visibleMaxY = std::min(cameraPos.y + halfWorldHeight + PADDING, height);

    m_impl->m_shader->use();
    m_impl->m_shader->setUniformsForBuiltins();
    
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position | kCCVertexAttribFlag_Color);
    ccGLBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    for (const auto& drawNode : m_impl->m_drawNodes) {
        if (drawNode->isEnabled()) {
            drawNode->draw(m_impl->m_drawGridLayer, visibleMinX, visibleMaxX, visibleMinY, visibleMaxY);
        }
    }
    
    batchDraw();

    glBlendFunc(oldSrc, oldDst);
}