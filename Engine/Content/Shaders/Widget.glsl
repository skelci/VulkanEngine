// Type UI
// Property vec4 Color 1 1 1 1
// Property texture Texture Engine/Textures/White.png

#ifdef FRAGMENT
vec4 GetColor() {
    return mat.Color * texture(Texture, UV);
}
#endif
