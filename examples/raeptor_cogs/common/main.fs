layout(location = 0) flat in int Type;
layout(location = 1) flat in int DataOffset;
layout(location = 2) flat in int writeMaskID;
layout(location = 3) flat in int readMaskID;
layout(location = 4) in vec2 vUV;
layout(location = 5) in vec3 vBarycentric;
layout(location = 6) in vec4 vClipPos;
layout(location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D uTextureSampler;
layout(binding = 1) uniform usampler2D uMaskTextureSampler;

void main() {
    uint maskValue = texture(uMaskTextureSampler, vClipPos.xy / vClipPos.w * 0.5 + 0.5).r;
    if (maskValue != uint(readMaskID)) {
        discard; // Discard transparent fragments
    }
    /*float edgeWidth = 1.0; // Adjust this value to change edge thickness
    vec3 d = fwidth(vBarycentric); 
    float minB = min(min(vBarycentric.x, vBarycentric.y), vBarycentric.z);
    float edgeFactor = smoothstep(0.0, max(d.x,d.y)*edgeWidth, minB);*/
    float edgeFactor = 1.0;
    vec4 outlineColor = vec4(0.0, 1.0, 0.0, 1.0); // Outline color (green)
    vec4 fillColor; // Fill color (black)
    switch (Type) {
        case RENDERER_MODE_2D_SPRITE:
            // Default rendering behavior
            fillColor.rgb = unpackVec3(DataOffset);
            fillColor = texture(uTextureSampler, vUV) * vec4(fillColor.rgb, 1.0);
            break;
        case RENDERER_MODE_2D_TEXT:
            // Custom rendering behavior for mode 2
            fillColor.rgb = unpackVec3(DataOffset);
            float smoothing = unpackFloat(DataOffset + 3);
            float dist = texture(uTextureSampler, vUV).r; // 0..1
            // Map distance around 0.5 = glyph edge
            float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, dist);
            fillColor = vec4(fillColor.rgb, alpha);
            break;
        default:
            // Fallback for any other mode
            fillColor = texture(uTextureSampler, vUV) * vec4(1.0);
    }
    FragColor = mix(outlineColor, fillColor, edgeFactor);
}