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
