#version 410

in vec3 vPosition;
in vec3 vNormal;
in vec3 vColor;

uniform mat4 viewMat;
uniform mat4 modelMat;

out vec4 fragColor;

const float ka = 0.1;
const float kd = 1.0;
const float factor = 1.5;

vec3 ambientColor = vec3(1.0, 1.0, 1.0);
vec3 diffuseColor = vColor;
vec3 lightPositionWorld  = vec3(0.0, 0.0, 40.0);

void main() {
    ambientColor = ka * ambientColor;

    vec3 lightPosition = (viewMat * vec4(lightPositionWorld, 1.0)).xyz;
    vec3 surfaceToLight = normalize(lightPosition - vPosition);
    float dotProd = dot(surfaceToLight, vNormal);
    dotProd = max(0.0, dotProd);
    vec3 eye = normalize(-(viewMat * vec4(0.0, 0.0, 0.0, 1.0)).xyz);
    float dotProd2 = dot(eye, vNormal);
    dotProd2 = max(0.0, dotProd2);
    diffuseColor = kd * diffuseColor * pow(dotProd, factor + 1) * pow(1.0 - dotProd2, 1 - factor);

    fragColor = vec4(ambientColor + diffuseColor, 1.0);
}
