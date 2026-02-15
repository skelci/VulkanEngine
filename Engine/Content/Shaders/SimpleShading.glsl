#ifdef FRAGMENT
// Property texture Texture Engine/Textures/White.png
// Property vec3 LightColor 1 1 1
// Property vec3 LightDirection -0.5 -1 -0.5
// Property vec4 Albedo 1 1 1 1

vec4 GetColor() {
    vec3 lightDir = -normalize(mat.LightDirection);
    float diff = max(dot(fragNormal, lightDir), 0.1);
    vec3 diffuse = diff * mat.LightColor;
    
    vec4 texColor = texture(Texture, fragTexCoord);
    return vec4(texColor.rgb * diffuse, texColor.a) * mat.Albedo;
}
#endif
