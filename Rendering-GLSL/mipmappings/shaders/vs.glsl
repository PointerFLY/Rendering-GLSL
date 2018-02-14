#version 410

in vec3 position;
in vec2 textureCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 vPosition;
out vec2 vTextureCoord;

void main() {
    mat4 viewModelMat = viewMat * modelMat;
    vPosition = (viewModelMat * vec4(position, 1.0)).xyz;
    vTextureCoord = textureCoord;
    gl_Position = projMat * viewModelMat * vec4(position, 1.0);
}
