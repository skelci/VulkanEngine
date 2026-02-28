#ifdef FRAGMENT
// Property texture Texture Engine/Textures/White.png
// Property vec4 Color 1 1 1 1
// Property float Translucent 1
// Property vec2 UVScale 1 1
// Property vec3 LightColor 1 1 1
// Property vec3 LightDirection -0.5 -0.5 -1
// Property vec3 AmbientLight 0.2 0.2 0.2
// Property float Roughness 0.5
// Property texture RoughnessMap Engine/Textures/White.png
// Property float FadeDistance -1

#include "Engine/Content/Shaders/Functions/Shade.glsl"

vec4 GetColor() {
    vec4 texColor = texture(Texture, UV * mat.UVScale) * mat.Color;
    vec3 shadedColor = Shade(
        texColor.rgb,
        normalize(mat.LightDirection),
        mat.LightColor,
        mat.AmbientLight,
        Normal,
        mat.Roughness * texture(RoughnessMap, UV * mat.UVScale).r
    );
    
    float alpha = (mat.Translucent > 0.5) ? texColor.a : 1.0;
    if (mat.FadeDistance > 0) {
        float distance = length(ModelWP - CameraWP);
        float fadeFactor = saturate((distance - mat.FadeDistance) / 5.0); // Fade out over 5 units beyond FadeDistance
        alpha *= one_minus(fadeFactor);
    }
    return vec4(shadedColor, alpha);
}
#endif
