#version 410

in vec3 vColor;
in vec3 vPosition;
in vec3 vNormal;

uniform vec3 cameraPosition;
uniform samplerCube skybox;

out vec4 fragColor;

void main() {
    vec3 surfaceToCamera = normalize(vPosition - cameraPosition);
    vec3 ratios = vec3(1.0 / 1.6,
                       1.0 / 1.45,
                       1.0 / 1.3);
    vec3 color;
    for (int i = 0; i < 3; i++) {
        vec3 reflectDir = refract(surfaceToCamera, vNormal, ratios[i]);
        color[i] = texture(skybox, reflectDir)[i];
    }
    fragColor = vec4(color, 1.0);
}
