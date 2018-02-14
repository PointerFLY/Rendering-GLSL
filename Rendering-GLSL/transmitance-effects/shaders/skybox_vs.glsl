#version 410

in vec3 position;

uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 vTextureCoord;

void main() {
    vTextureCoord = position;
    gl_Position = projMat * mat4(mat3(viewMat)) * vec4(position, 1.0);
}

