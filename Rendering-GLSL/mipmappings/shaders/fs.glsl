#version 410

in vec3 vPosition;
in vec2 vTextureCoord;

uniform sampler2D sampler;
uniform float mipmapLevel;

out vec4 fragColor;

void main() {
    fragColor = textureLod(sampler, vTextureCoord, mipmapLevel);
}

