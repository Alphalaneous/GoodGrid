#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../Color.hpp"

namespace alpha::grid {
    
class GOOD_GRID_API_DLL Bounds : public DrawGridBase {
public:
    static Bounds* create();
    void draw(const cocos2d::CCRect& visibleRect) override;

    void setTopBoundColor(const Color& color);
    void setBottomBoundColor(const Color& color);
    void setVerticalBoundColor(const Color& color);

    const Color& getTopBoundColor() const;
    const Color& getBottomBoundColor() const;
    const Color& getVerticalColor() const;

    static const Color& getDefaultTopBoundColor();
    static const Color& getDefaultBottomBoundColor();
    static const Color& getDefaultVerticalColor();

    void setTopBoundLineWidth(float width);
    void setBottomBoundLineWidth(float width);
    void setVerticalBoundLineWidth(float width);

    float getTopBoundLineWidth() const;
    float getBottomBoundLineWidth() const;
    float getVerticalLineWidth() const;

    static float getDefaultTopBoundLineWidth();
    static float getDefaultBottomBoundLineWidth();
    static float getDefaultVerticalLineWidth();

protected:
    bool init() override;

    Bounds();
    ~Bounds();

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}