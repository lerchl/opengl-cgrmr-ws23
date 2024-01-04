#pragma once

#include "index_buffer.h"
#include "shader.h"
#include "vertex_array.h"

class Renderer {
   public:
    void clear() const;
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};
