#pragma once

#include <ccTypes.h>
#include <Geode/cocos/shaders/CCGLProgram.h>
#include <Geode/Result.hpp>
#include "DrawNode.hpp"

#ifdef GEODE_IS_WINDOWS
    #ifdef GOOD_GRID_API_EXPORTING
        #define GOOD_GRID_API_DLL __declspec(dllexport)
    #else
        #define GOOD_GRID_API_DLL __declspec(dllimport)
    #endif
#else
    #define GOOD_GRID_API_DLL __attribute__((visibility("default")))
#endif

extern GOOD_GRID_API_DLL const float MAX_HEIGHT;
extern GOOD_GRID_API_DLL const float GROUND_OFFSET;
extern GOOD_GRID_API_DLL const float PADDING;

struct GOOD_GRID_API_DLL alignas(16) Vertex {
    cocos2d::ccVertex2F position;
    cocos2d::ccColor4B color;
    uint8_t padding[3];
};

class GOOD_GRID_API_DLL LineColor {
    cocos2d::ccColor4B m_colorA;
    cocos2d::ccColor4B m_colorB;
    bool m_hasColorB = false;

public:
    LineColor() = default;
    LineColor(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
    LineColor(const cocos2d::ccColor4B& colorA);
    LineColor(const cocos2d::ccColor4B& colorA, const cocos2d::ccColor4B& colorB);

    cocos2d::ccColor4B getColorA() const;
    cocos2d::ccColor4B getColorB() const;
};

struct DrawGridAPIImpl;

class GOOD_GRID_API_DLL DrawGridAPI {
    std::unique_ptr<DrawGridAPIImpl> m_impl;

protected:
    void addDrawInternal(const std::string& id, std::unique_ptr<DrawNode> drawNode);
    void ensureViewTransformValid();
    void batchDraw();
    std::array<Vertex, 6> rectToTriangles(const cocos2d::CCRect& rect, const cocos2d::ccColor4B& color);
    std::array<Vertex, 24> rectToBorderTriangles(const cocos2d::CCRect& rect, const cocos2d::ccColor4B& color, float width);
    const std::vector<std::unique_ptr<DrawNode>>& getDrawNodes();
    void setHideInvisible(bool enabled);

public:
    DrawGridAPI();
    ~DrawGridAPI();
    DrawGridAPI(const DrawGridAPI&) = delete;
    DrawGridAPI& operator=(const DrawGridAPI&) = delete;
    DrawGridAPI(DrawGridAPI&&) noexcept;
    DrawGridAPI& operator=(DrawGridAPI&&) noexcept;

    static DrawGridAPI& get();

    void init(DrawGridLayer* drawGridLayer, cocos2d::CCGLProgram* shader);
    void markDirty();
    void shouldSort();
    void sort();
    void setVanillaDraw(bool enabled);
    void setLineSmoothing(bool enabled);
    void setLineSmoothingLimit(float limit);
    void overrideGridBoundsSize(cocos2d::CCSize size);
    void overrideGridBoundsOrigin(cocos2d::CCPoint point);
    void generateTimeMarkers();
    void draw();
    void drawLine(const cocos2d::ccVertex2F& start, const cocos2d::ccVertex2F& end, const LineColor& color, float width, bool blend = false);
    void drawRect(const cocos2d::CCRect& rect, const cocos2d::ccColor4B& color, bool blend = false);
    void drawRectOutline(const cocos2d::CCRect& rect, const cocos2d::ccColor4B& color, float width, bool blend = false);

    cocos2d::CCSize getGridBoundsSize();
    cocos2d::CCPoint getGridBoundsOrigin();
    cocos2d::CCPoint getPortalMinMax(GameObject* obj);
    std::unordered_map<float, cocos2d::ccColor4B> getTimeMarkers();
    cocos2d::CCSize getWorldViewSize();
    float getOverdrawFactor();
    float getLineSmoothingLimit();
    bool isDirty();
    bool isVanillaDraw();
    bool hasLineSmoothing();
    bool isObjectVisible(GameObject* object);
    geode::Result<DrawNode&> getNodeByID(const std::string& id);

    template <typename T>
    requires std::is_base_of_v<DrawNode, T>
    geode::Result<T&> getNode(const std::string& id) {
        auto result = getNodeByID(id);
        if (!result)  return geode::Err("Could not find node with given ID");
        if (auto casted = geode::cast::typeinfo_cast<T*>(&result.unwrap())) {
            return geode::Ok(*casted);
        }
        return geode::Err("Node found, but invalid type");
    }

    template <typename T>
    requires std::is_base_of_v<DrawNode, T>
    geode::Result<T&> getNode() {
        for (const auto& drawNode : getDrawNodes()) {
            if (auto casted = geode::cast::typeinfo_cast<T*>(drawNode.get())) {
                return geode::Ok(*casted);
            }
        }
        return geode::Err("Node not found with type");
    }

    template <typename T, typename... Args>
    requires std::is_base_of_v<DrawNode, T>
    T& addDraw(const std::string& id, Args&&... args) { 
        auto node = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *node;
        addDrawInternal(id, std::move(node));
        return ref;
    }
};