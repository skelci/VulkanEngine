vec3 Shade(vec3 baseColor, vec3 lightDir, vec3 lightColor, vec3 ambientLight, vec3 normal, float roughness) {
    vec3 viewDir = normalize(CameraWP - FragWP);

    // Blinn-Phong Specular
    vec3 halfDir = normalize(-lightDir + viewDir);
    float specAngle = max(dot(normal, halfDir), 0.0);
    float shine = one_minus(roughness) * 128.0;
    float specular = pow(specAngle, shine);
    float specularFactor = specular * one_minus(roughness);

    float diffuse = max(dot(normal, -lightDir), 0.0);

    vec3 diffuseComponent = diffuse * baseColor * lightColor + ambientLight * baseColor;
    vec3 specularComponent = specularFactor * lightColor;
    return diffuseComponent + specularComponent;
}
