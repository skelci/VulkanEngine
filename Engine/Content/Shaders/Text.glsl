// Type UI
// Property texture Texture Engine/Textures/White.png
// Property vec4 Color 1 1 1 1

#ifdef VERTEX
vec3 GetWPO() {
    return vec3(VertRP.xy / WidgetSize - VertRP.xy - vec2(0.5, 0.0), 0.0);
}
#endif

#ifdef FRAGMENT
vec4 GetColor() {
    float alpha = texture(Texture, UV).a;
    return vec4(mat.Color.rgb, mat.Color.a * alpha);
}
#endif
