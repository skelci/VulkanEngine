#ifdef FRAGMENT
vec4 GetColor() {
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0));
    float diff = max(dot(fragNormal, lightDir), 0.1);
    vec3 diffuse = diff * vec3(1.0); // White light
    
    vec4 texColor = texture(DiffuseTexture, fragTexCoord);
    return vec4(texColor.rgb * diffuse, texColor.a);
}
#endif
