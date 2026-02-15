#ifdef VERTEX
vec3 GetWPO(vec3 pos) {
    return vec3(0.0);
}
#endif

#ifdef FRAGMENT
vec4 GetColor() {
    return vec4(fragColor * texture(DiffuseTexture, fragTexCoord).rgb, 1.0);
}
#endif
