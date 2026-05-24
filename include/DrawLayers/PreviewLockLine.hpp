#pragma once

#include "../DrawGridBase.hpp"
#include "../Export.hpp"

namespace good_grid {
    
    class GOOD_GRID_API_DLL PreviewLockLine : public DrawGridBase {
    public:
        static PreviewLockLine* create();
        void draw(float minX, float maxX, float minY, float maxY) override;

        void setLineColor(const cocos2d::ccColor4B& color);
        const cocos2d::ccColor4B& getLineColor() const;
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