#pragma once

#include <Geode/cocos/shaders/CCGLProgram.h>

#ifdef GEODE_IS_WINDOWS
    #ifdef GOOD_GRID_API_EXPORTING
        #define GOOD_GRID_API_DLL __declspec(dllexport)
    #else
        #define GOOD_GRID_API_DLL __declspec(dllimport)
    #endif
#else
    #define GOOD_GRID_API_DLL __attribute__((visibility("default")))
#endif

using DrawFunc = std::function<void(DrawGridLayer*, float, float, float, float)>;

struct DrawNodeImpl;

class GOOD_GRID_API_DLL DrawNode {
    std::unique_ptr<DrawNodeImpl> m_impl;
public:
    DrawNode();
    DrawNode(const std::string& id);
    DrawNode(const DrawNode&);
    DrawNode& operator=(const DrawNode&);
    DrawNode(DrawNode&&) noexcept;
    DrawNode& operator=(DrawNode&&) noexcept;
    virtual ~DrawNode();

    std::string getID() const;
    bool isEnabled() const;
    int getZOrder() const;
    void setZOrder(int order);
    void setEnabled(bool enabled);
    void setID(const std::string& id);
    virtual void init(DrawGridLayer* drawGridLayer);
    virtual void draw(DrawGridLayer* drawGridLayer, float minX, float maxX, float minY, float maxY);
};