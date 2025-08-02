#include "../include/DrawGridAPI.hpp"

struct DrawNodeImpl {
    std::string m_id;
    int m_zOrder = 0;
    bool m_enabled = true;
};

DrawNode::DrawNode() : m_impl(std::make_unique<DrawNodeImpl>()) {}

DrawNode::DrawNode(const std::string& id)
    : m_impl(std::make_unique<DrawNodeImpl>()) {
    m_impl->m_id = id;
}

DrawNode::DrawNode(const DrawNode& other)
    : m_impl(std::make_unique<DrawNodeImpl>(*other.m_impl)) {}

DrawNode& DrawNode::operator=(const DrawNode& other) {
    if (this != &other) m_impl = std::make_unique<DrawNodeImpl>(*other.m_impl);
    return *this;
}

DrawNode::DrawNode(DrawNode&&) noexcept = default;
DrawNode& DrawNode::operator=(DrawNode&&) noexcept = default;
DrawNode::~DrawNode() = default;

std::string DrawNode::getID() const {
    return m_impl->m_id;
}

bool DrawNode::isEnabled() const {
    return m_impl->m_enabled;
}

void DrawNode::setEnabled(bool enabled) {
    m_impl->m_enabled = enabled;
}

int DrawNode::getZOrder() const {
    return m_impl->m_zOrder;
}

void DrawNode::setZOrder(int order) {
    m_impl->m_zOrder = order;
    DrawGridAPI::get().shouldSort();
}

void DrawNode::setID(const std::string& id) {
    m_impl->m_id = id;
}

void DrawNode::draw(DrawGridLayer* dgl, float minX, float maxX, float minY, float maxY) {}

void DrawNode::init(DrawGridLayer* dgl) {}