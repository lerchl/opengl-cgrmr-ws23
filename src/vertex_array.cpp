#include "vertex_array.h"

#include "error_handling.h"
#include <ranges>

namespace ranges = std::ranges;

VertexArray::VertexArray() {
    GLCall(glGenVertexArrays(1, &rendererId));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &rendererId));
}

void VertexArray::bind() const {
    GLCall(glBindVertexArray(rendererId));
}

void VertexArray::unbind() const {
    GLCall(glBindVertexArray(0));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    bind();
    vb.bind();
    const auto& elements = layout.getElements();
    unsigned int offset = 0;

    for (auto i : ranges::views::iota(0, ranges::ssize(elements))) {
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*) offset));
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }

}
