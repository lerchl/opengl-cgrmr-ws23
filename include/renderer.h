#pragma once

#include "index_buffer.h"
#include "vertex_array.h"
#include "shader.h"

class Renderer {
   public:
    void clear() const;
    void draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};

