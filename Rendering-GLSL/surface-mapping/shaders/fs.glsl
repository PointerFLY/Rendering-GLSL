#version 410

in vec3 vColor;
in vec2 vTextureCoord;

uniform sampler2D sampler;

out vec4 fragColor;

void main() {
    fragColor = texture(sampler, vTextureCoord);
}
