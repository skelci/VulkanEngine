// Type UI
// Property vec4 Color 1 1 1 1
// Property texture Texture Engine/Textures/White.png
// Property float BorderWidth 0
// Property vec4 BorderColor 0 0 0 1
// Property vec4 CornerRadius 0 0 0 0

#ifdef FRAGMENT
float sdRoundedBox(vec2 p, vec2 b, vec4 r) {
    // Select radius based on quadrant
    r.xy = (p.x > 0.0) ? r.xy : r.zw;
    r.x  = (p.y > 0.0) ? r.x  : r.y;
    
    vec2 q = abs(p) - b + r.x;
    return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - r.x;
}

vec4 GetColor() {
    vec2 p = UV * WidgetSize;
    float d = sdRoundedBox(p - WidgetSize * 0.5, WidgetSize * 0.5, mat.CornerRadius);

    vec4 innerWidth = max(mat.CornerRadius - mat.BorderWidth * 0.75, 0.0);
    float innerD = sdRoundedBox(p - WidgetSize * 0.5, WidgetSize * 0.5 - mat.BorderWidth, innerWidth);

    float alpha = 1.0 - smoothstep(-0.5, 0.5, d);
    float borderMix = smoothstep(-0.5, 0.5, innerD);

    vec4 texColor = texture(Texture, UV);
    vec4 finalColor = lerp(mat.Color * texColor, mat.BorderColor, borderMix);

    return vec4(finalColor.rgb, finalColor.a * alpha);
}
#endif
