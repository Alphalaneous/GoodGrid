#pragma once
#include "DrawNode.hpp"
#include "DrawGridAPI.hpp"

#ifdef GEODE_IS_WINDOWS
    #ifdef GOOD_GRID_API_EXPORTING
        #define GOOD_GRID_API_DLL __declspec(dllexport)
    #else
        #define GOOD_GRID_API_DLL __declspec(dllimport)
    #endif
#else
    #define GOOD_GRID_API_DLL __attribute__((visibility("default")))
#endif

class GOOD_GRID_API_DLL Grid : public DrawNode {
    LineColor m_gridColor = { 0, 0, 0, 150 };
    int m_gridColorPriority = 0;
    float m_lineWidth = 1.0f;
    int m_lineWidthPriority = 0;

    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setGridColor(const LineColor& color, int priority = 0);
    int getGridColorPriority() const;
    const LineColor& getGridColor() const;

    void setLineWidth(float width, int priority = 0);
    int getLineWidthPriority() const;
    float getLineWidth() const;
};

class GOOD_GRID_API_DLL Bounds : public DrawNode {
    LineColor m_topBoundColor = { 255, 255, 255, 255 };
    LineColor m_bottomBoundColor = { 255, 255, 255, 255 };
    LineColor m_verticalBoundColor = { 255, 255, 255, 255 };
    int m_topBoundColorPriority = 0;
    int m_bottomBoundColorPriority = 0;
    int m_verticalBoundColorPriority = 0;

    float m_topBoundLineWidth = 2.0f;
    float m_bottomBoundLineWidth = 2.0f;
    float m_verticalBoundLineWidth = 1.0f;
    int m_topBoundLineWidthPriority = 0;
    int m_bottomBoundLineWidthPriority = 0;
    int m_verticalBoundLineWidthPriority = 0;

    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setTopBoundColor(const LineColor& color, int priority = 0);
    void setBottomBoundColor(const LineColor& color, int priority = 0);
    void setVerticalBoundColor(const LineColor& color, int priority = 0);

    int getTopBoundColorPriority() const;
    int getBottomBoundColorPriority() const;
    int getVerticalBoundColorPriority() const;

    const LineColor& getTopBoundColor() const;
    const LineColor& getBottomBoundColor() const;
    const LineColor& getVerticalColor() const;

    void setTopBoundLineWidth(float width, int priority = 0);
    void setBottomBoundLineWidth(float width, int priority = 0);
    void setVerticalBoundLineWidth(float width, int priority = 0);

    int getTopBoundLineWidthPriority() const;
    int getBottomBoundLineWidthPriority() const;
    int getVerticalBoundLineWidthPriority() const;

    float getTopBoundLineWidth() const;
    float getBottomBoundLineWidth() const;
    float getVerticalLineWidth() const;
};

class GOOD_GRID_API_DLL Ground : public DrawNode {
    LineColor m_topGroundColor = { 255, 175, 50, 255 };
    LineColor m_bottomGroundColor = { 255, 175, 50, 255 };
    int m_topGroundColorPriority = 0;
    int m_bottomGroundColorPriority = 0;

    float m_topGroundLineWidth = 2.0f;
    float m_bottomGroundLineWidth = 2.0f;
    int m_topGroundLineWidthPriority = 0;
    int m_bottomGroundLineWidthPriority = 0;

    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setTopGroundColor(const LineColor& color, int priority = 0);
    void setBottomGroundColor(const LineColor& color, int priority = 0);

    int getTopGroundColorPriority() const;
    int getBottomGroundColorPriority() const;

    const LineColor& getTopGroundColor() const;
    const LineColor& getBottomGroundColor() const;

    void setTopGroundLineWidth(float width, int priority = 0);
    void setBottomGroundLineWidth(float width, int priority = 0);

    int getTopGroundLineWidthPriority() const;
    int getBottomGroundLineWidthPriority() const;

    float getTopGroundLineWidth() const;
    float getBottomGroundLineWidth() const;
};

class GOOD_GRID_API_DLL GuideObjects : public DrawNode {
    std::map<int, std::vector<std::function<void(LineColor& bottomColor, LineColor& topColor, EffectGameObject* object, float& lineWidthBottom, float& lineWidthTop)>>> m_colorsForObject;
    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setPropertiesForObject(std::function<void(LineColor& bottomColor, LineColor& topColor, EffectGameObject* object, float& lineWidthBottom, float& lineWidthTop)> colorForObject, int priority = 0);
};

class GOOD_GRID_API_DLL EffectLines : public DrawNode {
    std::map<int, std::vector<std::function<void(LineColor& color, float& x, EffectGameObject* object, float& lineWidth)>>> m_colorsForObject;
    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setPropertiesForObject(std::function<void(LineColor& color, float& x, EffectGameObject* object, float& lineWidth)> colorForObject, int priority = 0);
};

class GOOD_GRID_API_DLL DurationLines : public DrawNode {
    std::map<int, std::vector<std::function<void(LineColor& color, EffectGameObject* object, float& lineWidth)>>> m_colorsForObject;
    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setPropertiesForObject(std::function<void(LineColor& color, EffectGameObject* object, float& lineWidth)> colorForObject, int priority = 0);
};

class GOOD_GRID_API_DLL Guidelines : public DrawNode {
    std::map<int, std::vector<std::function<void(LineColor& color, float& value, float& lineWidth)>>> m_colorsForValue;
    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setPropertiesForValue(std::function<void(LineColor& color, float& value, float& lineWidth)> colorForValue, int priority = 0);
};

class GOOD_GRID_API_DLL BPMTriggers : public DrawNode {
    std::map<int, std::vector<std::function<void(LineColor& color, AudioLineGuideGameObject* object, float& x, int beat, int beatsPerBar, float& lineWidth)>>> m_colorsForBeats;
    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setPropertiesForBeats(std::function<void(LineColor& color, AudioLineGuideGameObject* object, float& x, int beat, int beatsPerBar, float& lineWidth)> colorForBeats, int priority = 0);
};

class GOOD_GRID_API_DLL AudioLine : public DrawNode {
    std::map<int, std::vector<std::function<void(LineColor& color, bool playback, float time, const cocos2d::CCPoint& position, float& lineWidth)>>> m_colorsForTime;
    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setPropertiesForTime(std::function<void(LineColor& color, bool playback, float time, const cocos2d::CCPoint& position, float& lineWidth)> colorForTime, int priority = 0);
};

class GOOD_GRID_API_DLL PositionLines : public DrawNode {
    LineColor m_verticalLineColor = { 0, 0, 0, 50 };
    LineColor m_horizontalLineColor = { 0, 0, 0, 50 };
    int m_verticalLineColorPriority = 0;
    int m_horizontalLineColorPriority = 0;

    float m_verticalLineWidth = 2.0f;
    float m_horizontalLineWidth = 2.0f;

    int m_verticalLineWidthPriority = 0;
    int m_horizontalLineWidthPriority = 0;

    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setVerticalLineColor(const LineColor& color, int priority = 0);
    void setHorizontalLineColor(const LineColor& color, int priority = 0);

    int getVerticalLineColorPriority() const;
    int getHorizontalLineColorPriority() const;

    const LineColor& getVerticalLineColor() const;
    const LineColor& getHorizontalLineColor() const;

    void setVerticalLineWidth(float width, int priority = 0);
    void setHorizontalLineWidth(float width, int priority = 0);

    int getVerticalLineWidthPriority() const;
    int getHorizontalLineWidthPriority() const;

    float getVerticalLineWidth() const;
    float getHorizontalLineWidth() const;
};

class GOOD_GRID_API_DLL PreviewLockLine : public DrawNode {
    LineColor m_lineColor = { 255, 150, 0, 255 };
    int m_lineColorPriority = 0;

    float m_lineWidth = 2.0f;
    int m_lineWidthPriority = 0;

    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setLineColor(const LineColor& color, int priority = 0);

    int getLineColorPriority() const;

    const LineColor& getLineColor() const;

    void setLineWidth(float width, int priority = 0);

    int getLineWidthPriority() const;

    float getLineWidth() const;
};