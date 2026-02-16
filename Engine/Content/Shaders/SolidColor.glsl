#ifdef FRAGMENT
// Property vec4 Color 1 1 1 1
// Property texture Texture Engine/Textures/White.png
vec4 GetColor() {
    return vec4(FragColor * texture(Texture, UV).rgb, 1.0) * mat.Color;
}
#endif
