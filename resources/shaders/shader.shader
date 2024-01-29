#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoordinates;

out vec4 v_position;
out vec2 v_textureCoordinates;

uniform mat4 u_modelViewProjectionMatrix;

void main() {
    gl_Position = u_modelViewProjectionMatrix * position;
    v_position = gl_Position;
    v_textureCoordinates = textureCoordinates;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec4 v_position;
in vec2 v_textureCoordinates;

uniform sampler2D u_texture;
uniform vec3 u_lightPosition;

void main() {
    vec3 lightDirection = normalize(u_lightPosition - v_position.xyz);
    float lightIntensity = max(dot(normalize(v_position.xyz), lightDirection), 0.0);
    vec4 tex = texture(u_texture, v_textureCoordinates);
    color = vec4(tex.rgb * lightIntensity, tex.a);
};
