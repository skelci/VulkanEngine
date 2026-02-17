#ifdef FRAGMENT
// Property texture Texture Engine/Textures/White.png
// Property vec3 LightColor 1 1 1
// Property vec3 LightDirection -0.5 -1 -0.5
// Property vec4 Albedo 1 1 1 1
// Property float Roughness 0.5

vec4 GetColor() {
    vec3 lightDir = -normalize(mat.LightDirection);
    vec3 viewDir = normalize(CameraWP - FragWP);

    // Blinn-Phong Specular
    vec3 halfDir = normalize(lightDir + viewDir);
    float specAngle = max(dot(Normal, halfDir), 0.0);
    float shine = one_minus(mat.Roughness) * 128.0;
    float specular = pow(specAngle, shine);
    float specularFactor = specular * one_minus(mat.Roughness);

    float diff = max(dot(Normal, lightDir), 0.1);
    
    vec4 texColor = texture(Texture, UV) * mat.Albedo;
    vec3 diffuseComponent = diff * texColor.rgb * mat.LightColor;
    vec3 specularComponent = specularFactor * mat.LightColor;
    return vec4(diffuseComponent + specularComponent, texColor.a);
}
#endif
