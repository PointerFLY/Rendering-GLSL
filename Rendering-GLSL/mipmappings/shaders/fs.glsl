#version 410

in vec3 vPosition;
in vec2 vTextureCoord;

uniform mat4 viewMat;
uniform mat4 modelMat;
uniform sampler2D sampler;
uniform sampler2D normalMap;
uniform bool isBumped;

out vec4 fragColor;

const float ka = 0.1;
const float kd = 1.0;
const float ks = 1.0;
const float specularExponent = 6;

vec3 ambientColor = vec3(1.0, 1.0, 1.0);
vec3 diffuseColor;
vec3 specularColor = vec3(1.0, 1.0, 1.0);
vec3 lightPositionOrigin  = vec3(0.0, 0.0, 0.5);

void main() {
    vec3 normal = vec3(0.0, 0.0, 1.0);
    if (isBumped) {
        normal = texture(normalMap, vTextureCoord).rgb;
        normal = normalize(normal * 2.0 - 1.0);
    }
    
    ambientColor = ambientColor * ka;
    
    diffuseColor = texture(sampler, vTextureCoord).rgb;
    vec3 lightPosition = (modelMat * viewMat * vec4(lightPositionOrigin, 1.0)).xyz;
    vec3 surfaceToLight = normalize(lightPosition - vPosition);
    float dotProd = dot(surfaceToLight, normal);
    dotProd = max(0.0, dotProd);
    diffuseColor = diffuseColor * dotProd * kd;
    
    vec3 reflection = normalize(reflect(-lightPosition, normal));
    vec3 eye = normalize(-(viewMat * vec4(0.0, 0.0, 0.0, 1.0)).xyz);
    dotProd = dot(reflection, eye);
    dotProd = max(0.0, dotProd);
    float specularFactor = pow(dotProd, specularExponent);
    specularColor = specularColor * specularFactor * ks;
    
    fragColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
}
