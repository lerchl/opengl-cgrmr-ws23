#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoordinates;

out vec2 v_textureCoordinates;

void main() {
    gl_Position = position;
    v_textureCoordinates = textureCoordinates;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_textureCoordinates;

uniform vec4 u_color;
uniform sampler2D u_texture;

void main() {
    color = texture(u_texture, v_textureCoordinates) * u_color;
};
