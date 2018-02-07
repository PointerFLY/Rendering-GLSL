#version 410

in vec3 position;
in vec3 normal;
in vec2 textureCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 vColor;

void main() {
    vColor = (viewMat * modelMat * vec4(normal, 0.0)).xyz;
    gl_Position =  projMat * viewMat * modelMat * vec4(position, 1.0);
}
