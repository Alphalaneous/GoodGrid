#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"
#include "../Color.hpp"

namespace alpha::grid {
    
class GOOD_GRID_API_DLL Grid : public DrawGridBase {
public:
    static Grid* create();
    void draw(float minX, float maxX, float minY, float maxY) override;

    void setGridColor(const Color& color);
    const Color& getGridColor() const;

    void setLineWidth(float width);
    float getLineWidth() const;

protected:
    bool init() override;

    Grid();
    ~Grid();

    class Impl;
    std::unique_ptr<Impl> m_impl;
};

}