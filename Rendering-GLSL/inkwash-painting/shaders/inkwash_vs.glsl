#version 410

in vec3 position;
in vec3 normal;
in vec2 textureCoord;

out VertexOutput {
    vec3 worldPosition;
    vec3 toCamera;
    vec3 normal;
    float vn;
} vOut;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;
uniform vec3 cameraPosition;

void main() {
    gl_Position = projMat * viewMat * modelMat * vec4(position, 1.0);
    vOut.worldPosition = (modelMat * vec4(position, 1.0)).xyz;
    vOut.toCamera = normalize(cameraPosition - vOut.worldPosition);
    vOut.normal = (normalize(modelMat * vec4(normal, 0.0))).xyz;
    vOut.vn = dot(vOut.toCamera, vOut.normal);
}
