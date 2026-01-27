layout(location = 0) in vec2 vertexPos;
layout(location = 1) in vec2 vertexUV;

layout(location = 0) out vec2 vUV;
layout(location = 1) out vec3 vBarycentric; // Normal for UV mapping
layout(location = 2) out vec4 vClipPos;
layout(location = 3) flat out int Type; // Instance type
layout(location = 4) flat out int DataOffset; // Offset into another SSBO or same buffer
layout(location = 5) flat out int readMaskID; // Reading mask ID
layout(location = 6) flat out int writeMaskID; // Writing mask ID

uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform float uTime; 

void main() {
    #if defined(HAVE_BASE_INSTANCE)
    InstanceGPUData instance = instances[instanceIndices[gl_InstanceID + gl_BaseInstance]];
    #else
    InstanceGPUData instance = getInstanceData(getInstanceID(getBaseInstance(), gl_InstanceID));
    #endif
    if (instance.type == 0) {
        gl_Position = vec4(2.0, 2.0, 2.0, 1.0);
        return;
    }

    vClipPos = uProjectionMatrix * uViewMatrix * instance.model * vec4(vertexPos, 0.0, 1.0);
    gl_Position = vClipPos; // Slight animation for testing
    //gl_Position.xy += sin(uTime + float(gl_InstanceID)) * 0.01;
    Type = instance.type;

    // UV coordinates
    vUV = instance.uv.xy + vertexUV * instance.uv.zw;

    // Assign barycentric coordinates
    /*if (vertexID == 0) vBarycentric = vec3(1,0,0);
    else if (vertexID == 1) vBarycentric = vec3(0,1,0);
    else vBarycentric = vec3(0,0,1);*/
    vBarycentric = vec3(1.0); // Not used for now

    DataOffset = instance.dataOffset;
    readMaskID = instance.readMaskID;
    writeMaskID = instance.writeMaskID;
}