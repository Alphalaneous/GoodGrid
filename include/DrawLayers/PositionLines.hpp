#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../Color.hpp"

namespace alpha::grid {
    
class GOOD_GRID_API_DLL PositionLines : public DrawGridBase {
public:
    static PositionLines* create();
    void draw(float minX, float maxX, float minY, float maxY) override;

    void setVerticalLineColor(const Color& color);
    void setHorizontalLineColor(const Color& color);

    const Color& getVerticalLineColor() const;
    const Color& getHorizontalLineColor() const;

    void setVerticalLineWidth(float width);
    void setHorizontalLineWidth(float width);

    float getVerticalLineWidth() const;
    float getHorizontalLineWidth() const;

protected:
    bool init() override;

    PositionLines();
    ~PositionLines();

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}