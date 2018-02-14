#version 410

in vec3 vColor;
in vec3 vPosition;
in vec3 vNormal;

uniform vec3 cameraPosition;
uniform samplerCube skybox;

out vec4 fragColor;

void main() {
    float ratio = 1.00 / 1.52;
    vec3 surfaceToCamera = normalize(vPosition - cameraPosition);
    vec3 reflectDir = refract(surfaceToCamera, vNormal, ratio);
    fragColor = vec4(texture(skybox, reflectDir).rgb, 1.0);
}
