// Property texture WaterNoise Game/Textures/WaterNoise.jpg

float noise(vec2 uv, float e, float offset) {
    float noise1 = texture(WaterNoise, uv * 0.02 + vec2(Time * 0.02, Time * 0.005)).r;
    float noise2 = texture(WaterNoise, uv * 0.02 + vec2(Time * 0.015, Time * -0.01)).r;
    return (pow(noise1 + offset, e) + pow(noise2 + offset, e)) * 0.5;
}

#ifdef VERTEX
vec3 GetWPO() {
    return vec3(0.0, 0.0, pow(noise(FragWP.xy, 1.0, 0.0), 0.3) * 1.0);
}
#endif

#ifdef FRAGMENT
#include "Engine/Content/Shaders/Functions/Shade.glsl"

float getHeight(vec2 uv) {
    return pow(noise(uv, 1.0, 0.0), 0.3) * 2.0;
}

vec4 GetColor() {
    float epsilon = 0.5;
    float h = getHeight(FragWP.xy);
    float hx = getHeight(FragWP.xy + vec2(epsilon, 0.0));
    float hy = getHeight(FragWP.xy + vec2(0.0, epsilon));

    vec3 normal = normalize(vec3(h - hx, h - hy, epsilon));

    vec3 baseColor = lerp(vec3(0.0, 0.3, 0.9), vec3(0.5, 0.8, 1.0), pow(noise(FragWP.xy, 5.0, 0.3) + 0.3, 1.0)); 
    vec3 color = Shade(
        baseColor,
        normalize(vec3(-0.5, -0.5, -1.0)), // Light direction
        vec3(1.0), // Light color
        vec3(0.2), // Ambient light
        normal,
        0.1 // Roughness
    );
    return vec4(color, 0.7); // Semi-transparent water
}
#endif
