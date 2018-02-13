#version 410

in vec3 vPosition;
in vec2 vTextureCoord;

uniform mat4 viewMat;
uniform mat4 modelMat;
uniform sampler2D normalMap;

out vec4 fragColor;

const float ka = 0.1;
const float kd = 1.0;
const float ks = 1.0;
const float specularExponent = 6;

vec3 ambientColor = vec3(1.0, 1.0, 1.0);
vec3 diffuseColor = vec3(1.0, 0.0, 0.0);
vec3 specularColor = vec3(1.0, 1.0, 1.0);
vec3 lightPositionWorld  = vec3(0.0, 0.0, 40.0);

void main() {
    vec3 normal = texture(normalMap, vTextureCoord).rgb;
    // 将法线向量转换为范围[-1,1]
    normal = normalize(normal * 2.0 - 1.0);
    
    ambientColor = ambientColor * ka;
    
    vec3 lightPosition = (viewMat * vec4(lightPositionWorld, 1.0)).xyz;
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
