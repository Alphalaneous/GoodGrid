#pragma once

#include "DrawGridLayer.hpp"
#include <Geode/binding/LevelEditorLayer.hpp>
#include <Geode/cocos/cocoa/CCGeometry.h>
#include "Vertex.hpp"

namespace good_grid::utils {

    inline std::array<Vertex, 6> rectToTriangles(const cocos2d::CCRect& rect, const cocos2d::ccColor4B& color) {
        float x = rect.getMinX();
        float y = rect.getMinY();
        float w = rect.size.width;
        float h = rect.size.height;

        return {
            Vertex{x,     y, color},
            Vertex{x + w, y, color},
            Vertex{x + w, y + h, color},

            Vertex{x,     y, color},
            Vertex{x + w, y + h, color},
            Vertex{x,     y + h, color}
        };
    }

    template <typename Fn>
    struct PriorityCallbackList {
        std::map<int, std::vector<Fn>> byPriority;
        std::vector<Fn> flat;
        bool dirty = false;

        void add(Fn fn, int priority) {
            byPriority[priority].push_back(fn);
            dirty = true;
        }

        void rebuildIfNeeded() {
            if (!dirty) return;
            flat.clear();
            for (auto& [_, vec] : byPriority) {
                flat.insert(flat.end(), vec.begin(), vec.end());
            }
            dirty = false;
        }

        bool empty() const {
            return byPriority.empty();
        }
    };

    inline MyDrawGridLayer* getDrawGridLayer() {
        auto editor = LevelEditorLayer::get();
        if (!editor) return nullptr;

        return static_cast<MyDrawGridLayer*>(editor->m_drawGridLayer);
    }
}