#version 410

in vec3 vColor;
in vec3 vTextureCoord;

uniform samplerCube skybox;

out vec4 fragColor;

void main() {
    fragColor = texture(skybox, vTextureCoord);
}

