#ifdef FRAGMENT
// Property vec4 Color 1 1 1 1
// Property texture Texture Engine/Textures/White.png
vec4 GetColor() {
    return vec4(fragColor * texture(Texture, fragTexCoord).rgb, 1.0) * mat.Color;
}
#endif
