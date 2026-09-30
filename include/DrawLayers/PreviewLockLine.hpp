#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../Color.hpp"

namespace alpha::grid {
    
class GOOD_GRID_API_DLL PreviewLockLine : public DrawGridBase {
public:
    static PreviewLockLine* create();
    void draw(const cocos2d::CCRect& visibleRect) override;

    void setLineColor(const Color& color);
    const Color& getLineColor() const;
    void setLineWidth(float width);
    float getLineWidth() const;

protected:
    bool init() override;

    PreviewLockLine();
    ~PreviewLockLine();

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}