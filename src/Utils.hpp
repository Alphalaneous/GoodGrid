#pragma once

#include "DrawGridLayer.hpp"
#include "Geode/utils/ZStringView.hpp"
#include <Geode/binding/LevelEditorLayer.hpp>
#include <Geode/cocos/cocoa/CCGeometry.h>

namespace alpha::grid::utils {

template <typename Fn>
class PriorityCallbackList {
public:
    struct Data {
        std::string m_id;
        int m_priority;
        Fn m_item;
    };

    void add(ZStringView ID, Fn fn, int priority) {
        auto [it, inserted] = m_data.try_emplace(ID, ID, priority, std::move(fn));

        auto& data = it->second;

        m_byPriority[priority].push_back(&data);
        m_dirty = true;
    }

    void remove(ZStringView ID) {
        auto iter = m_data.find(ID);
        if (iter == m_data.end()) return;

        auto& data = iter->second;
        auto& vec = m_byPriority[data.m_priority];
        
        std::erase_if(vec, [ID](auto* data) {
            return data->m_id == ID;
        });

        m_data.erase(iter);

        m_dirty = true;
    }

    void rebuildLazy() {
        if (!m_dirty) return;

        m_flat.clear();

        for (auto& [_, vec] : m_byPriority) {
            for (auto& data : vec) {
                m_flat.push_back(&data->m_item);
            }
        }
        m_dirty = false;
    }

    std::span<Fn*> all() {
        return m_flat;
    }

protected:
    StringMap<Data> m_data;
    std::map<int, std::vector<Data*>> m_byPriority;

    std::vector<Fn*> m_flat;
    bool m_dirty = false;
};

inline MyDrawGridLayer* getDrawGridLayer() {
    auto editor = LevelEditorLayer::get();
    if (!editor) return nullptr;

    return static_cast<MyDrawGridLayer*>(editor->m_drawGridLayer);
}

}