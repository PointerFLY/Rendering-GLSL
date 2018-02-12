#version 410

in vec3 position;

uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 vTextureCoord;

void main() {
    vTextureCoord = position;
    gl_Position =  projMat * viewMat * vec4(position, 1.0);
}

