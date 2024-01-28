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
uniform sampler2D u_normalMap;
uniform float u_time; // Time variable for scrolling

void main() {
    // Scroll the texture and normal map vertically based on time with modulo to avoid streaks
    vec2 scrolledCoordinates = vec2(v_textureCoordinates.x, mod(v_textureCoordinates.y + u_time, 1.0));

    // Fetch the color from the texture
    vec3 baseColor = texture(u_texture, scrolledCoordinates).rgb;

    // Fetch the normal from the normal map
    vec3 normalMap = texture(u_normalMap, scrolledCoordinates).xyz;

    // Convert normal map values from [0, 1] to [-1, 1]
    normalMap = normalize(normalMap * 2.0 - 1.0);

    // Apply normal mapping to get the perturbed normal
    vec3 normal = normalize(normalMap);

    // Simple lighting calculation
    float lighting = dot(normal, normalize(vec3(1.0, 1.0, 1.0)));

    // Combine base color with lighting
    vec3 finalColor = baseColor * lighting;

    // Output the final color
    color = vec4(finalColor, .5);
}



