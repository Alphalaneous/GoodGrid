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
    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setGridColor(const LineColor& color, int priority = 0);
    int getGridColorPriority() const;
    const LineColor& getGridColor() const;
};

class GOOD_GRID_API_DLL Bounds : public DrawNode {
    LineColor m_topBoundColor = { 255, 255, 255, 255 };
    LineColor m_bottomBoundColor = { 255, 255, 255, 255 };
    LineColor m_verticalBoundColor = { 255, 255, 255, 255 };
    int m_topBoundColorPriority = 0;
    int m_bottomBoundColorPriority = 0;
    int m_verticalBoundColorPriority = 0;

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
};

class GOOD_GRID_API_DLL Ground : public DrawNode {
    LineColor m_topGroundColor = { 255, 175, 50, 255 };
    LineColor m_bottomGroundColor = { 255, 175, 50, 255 };
    int m_topGroundColorPriority = 0;
    int m_bottomGroundColorPriority = 0;

    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setTopGroundColor(const LineColor& color, int priority = 0);
    void setBottomGroundColor(const LineColor& color, int priority = 0);

    int getTopGroundColorPriority() const;
    int getBottomGroundColorPriority() const;

    const LineColor& getTopGroundColor() const;
    const LineColor& getBottomGroundColor() const;
};

class GOOD_GRID_API_DLL GuideObjects : public DrawNode {
    std::map<int, std::vector<std::function<void(LineColor& bottomColor, LineColor& topColor, EffectGameObject* object)>>> m_colorsForObject;
    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setColorForObject(std::function<void(LineColor& bottomColor, LineColor& topColor, EffectGameObject* object)> colorForObject, int priority = 0);
};

class GOOD_GRID_API_DLL EffectLines : public DrawNode {
    std::map<int, std::vector<std::function<void(LineColor& color, EffectGameObject* object)>>> m_colorsForObject;
    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setColorForObject(std::function<void(LineColor& color, EffectGameObject* object)> colorForObject, int priority = 0);
};

class GOOD_GRID_API_DLL DurationLines : public DrawNode {
    std::map<int, std::vector<std::function<void(LineColor& color, EffectGameObject* object)>>> m_colorsForObject;
    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setColorForObject(std::function<void(LineColor& color, EffectGameObject* object)> colorForObject, int priority = 0);
};

class GOOD_GRID_API_DLL Guidelines : public DrawNode {
    std::map<int, std::vector<std::function<void(LineColor& color, float value)>>> m_colorsForValue;
    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setColorForValue(std::function<void(LineColor& color, float value)> colorForValue, int priority = 0);
};

class GOOD_GRID_API_DLL BPMTriggers : public DrawNode {
    std::map<int, std::vector<std::function<void(LineColor& color, AudioLineGuideGameObject* object, int beat, int beatsPerBar)>>> m_colorsForBeats;
    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setColorForBeats(std::function<void(LineColor& color, AudioLineGuideGameObject* object, int beat, int beatsPerBar)> colorForBeats, int priority = 0);
};

class GOOD_GRID_API_DLL AudioLine : public DrawNode {
    std::map<int, std::vector<std::function<void(LineColor& color, bool playback, float time, const cocos2d::CCPoint& position)>>> m_colorsForTime;
    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setColorForTime(std::function<void(LineColor& color, bool playback, float time, const cocos2d::CCPoint& position)> colorForTime, int priority = 0);
};

class GOOD_GRID_API_DLL PositionLines : public DrawNode {
    LineColor m_verticalLineColor = { 0, 0, 0, 50 };
    LineColor m_horizontalLineColor = { 0, 0, 0, 50 };
    int m_verticalLineColorPriority = 0;
    int m_horizontalLineColorPriority = 0;

    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setVerticalLineColor(const LineColor& color, int priority = 0);
    void setHorizontalLineColor(const LineColor& color, int priority = 0);

    int getVerticalLineColorPriority() const;
    int getHorizontalLineColorPriority() const;

    const LineColor& getVerticalLineColor() const;
    const LineColor& getHorizontalLineColor() const;
};

class GOOD_GRID_API_DLL PreviewLockLine : public DrawNode {
    LineColor m_lineColor = { 255, 150, 0, 255 };
    int m_lineColorPriority = 0;

    void draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY);
public:
    void setLineColor(const LineColor& color, int priority = 0);

    int getLineColorPriority() const;

    const LineColor& getLineColor() const;
};