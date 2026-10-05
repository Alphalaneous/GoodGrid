#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../Color.hpp"

namespace alpha::grid {
    
class GOOD_GRID_API_DLL Grid : public DrawGridBase {
public:
    static Grid* create();
    void draw(const cocos2d::CCRect& visibleRect) override;

    void setGridColor(const Color& color);
    const Color& getGridColor() const;
    static const Color& getDefaultGridColor();

    void setLineWidth(float width);
    float getLineWidth() const;
    static float getDefaultLineWidth();

protected:
    bool init() override;

    Grid();
    ~Grid();

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}