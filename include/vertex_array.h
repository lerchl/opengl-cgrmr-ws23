#pragma once

#include "vertex_buffer.h"

#include "vertex_buffer_layout.h"

class VertexArray {
   public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

   private:
    unsigned int rendererId;
};
