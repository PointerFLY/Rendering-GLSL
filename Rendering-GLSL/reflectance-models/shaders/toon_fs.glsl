#version 410

in vec3 vPosition;
in vec3 vNormal;
in vec3 vColor;

out vec4 fragColor;

uniform mat4 viewMat;
uniform mat4 modelMat;

const float kd = 1.0;

vec3 diffuseColor = vColor;
vec3 lightPositionWorld  = vec3(0.0, 0.0, 40.0);

void main() {
    vec3 lightPosition = (viewMat * vec4(lightPositionWorld, 1.0)).xyz;
    vec3 surfaceToLight = normalize(lightPosition - vPosition);
    float cosine = dot(surfaceToLight, vNormal);
    vec4 diff;

    if (cosine > 0.95) {
        diff = vec4(1.0, 1.0, 1.0, 1.0);
    } else if (cosine > 0.5) {
        diff = vec4(0.8, 0.8, 0.8, 1.0);
    } else if (cosine > 0.25) {
        diff = vec4(0.6, 0.6, 0.6, 1.0);
    } else {
        diff = vec4(0.2, 0.2, 0.2, 1.0);
    }

    fragColor = kd * diff * vec4(diffuseColor, 1.0);
}
