#version 410

in vec3 vPosition;
in vec3 vNormal;

uniform vec3 cameraPosition;
uniform samplerCube skybox;

out vec4 fragColor;

void main() {
    vec3 surfaceToCamera = normalize(vPosition - cameraPosition);
    vec3 reflectDir = reflect(surfaceToCamera, vNormal);
    fragColor = vec4(texture(skybox, reflectDir).rgb, 1.0);
}
