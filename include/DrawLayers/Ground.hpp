#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../Color.hpp"

namespace alpha::grid {

class GOOD_GRID_API_DLL Ground : public DrawGridBase {
public:
    static Ground* create();
    void draw(const cocos2d::CCRect& visibleRect) override;

    void setTopGroundColor(const Color& color);
    void setBottomGroundColor(const Color& color);

    const Color& getTopGroundColor() const;
    const Color& getBottomGroundColor() const;

    void setTopGroundLineWidth(float width);
    void setBottomGroundLineWidth(float width);

    float getTopGroundLineWidth() const;
    float getBottomGroundLineWidth() const;

    float getMinPortalY();
    float getMaxPortalY();

protected:
    bool init() override;

    Ground();
    ~Ground();

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}