#version 410

in vec3 vColor;
in vec3 vPosition;
in vec3 vNormal;

uniform vec3 cameraPosition;
uniform samplerCube skybox;

out vec4 fragColor;

void main() {
    vec3 surfaceToCamera = normalize(vPosition - cameraPosition);
    
    vec3 reflectDir = reflect(surfaceToCamera, vNormal);
    vec4 reflectColor = vec4(texture(skybox, reflectDir).rgb, 1.0);
    
    float ratio = 1.00 / 1.52;
    vec3 refractDir = refract(surfaceToCamera, vNormal, ratio);
    vec4 refractColor = vec4(texture(skybox, refractDir).rgb, 1.0);
    
    float reflectRatio = 0.5;
    fragColor = reflectColor * reflectRatio + refractColor * (1.0 - reflectRatio);
}
