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

uniform sampler2D u_texture;
uniform sampler2D u_normalMap; // New normal map texture

void main() {
    // Fetch the color from the texture
    vec3 baseColor = texture(u_texture, v_textureCoordinates).rgb;

    // Fetch the normal from the normal map
    vec3 normalMap = texture(u_normalMap, v_textureCoordinates).xyz;

    // Convert normal map values from [0, 1] to [-1, 1]
    normalMap = normalize(normalMap * 2.0 - 1.0);

    // Apply normal mapping to get the perturbed normal
    vec3 normal = normalize(normalMap);

    // Example: Simple lighting calculation (replace this with your lighting model)
    float lighting = dot(normal, normalize(vec3(1.0, 1.0, 1.0)));

    // Combine base color with lighting
    vec3 finalColor = baseColor * lighting;

    color = vec4(finalColor, 1.0);
};
