// Type UI
// Property texture Texture Engine/Textures/White.png
// Property vec4 Color 1 1 1 1

#ifdef FRAGMENT
vec4 GetColor() {
    float alpha = texture(Texture, UV).a;
    return vec4(mat.Color.rgb, mat.Color.a * alpha);
}
#endif
