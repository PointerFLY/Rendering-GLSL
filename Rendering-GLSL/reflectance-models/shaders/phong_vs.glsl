#version 410

in vec3 position;
in vec3 normal;
in vec2 textureCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 vPosition;
out vec3 vNormal;
out vec3 vColor;

void main() {
    mat4 viewModelMat = viewMat * modelMat;
    vPosition = (viewModelMat * vec4(position, 1.0)).xyz;
    vNormal = (viewModelMat * vec4(normal, 0.0)).xyz;
    vColor = vNormal;
    gl_Position = projMat * viewModelMat * vec4(position, 1.0);
}
