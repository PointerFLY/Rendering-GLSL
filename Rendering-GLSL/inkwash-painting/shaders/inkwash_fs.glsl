#version 410

struct LightBase {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight {
    LightBase base;
    vec3 direction;
};

in GeometryOutput {
    vec3 worldPosition;
    vec3 toCamera;
    vec3 normal;
    vec2 textureCoord;
    float strokePressure;
    float strokeOrientation;
} gIn;

uniform DirectionalLight directionalLight;
uniform float specularIntensity;  // specular reflectivity coefficient
uniform float specularPower;  // shininess or phong exponent

uniform sampler2D brushTexture;
uniform sampler2D inkTexture;

out vec4 fragColor;

vec4 computeLight(LightBase light, vec3 lightDirection) {
    vec4 ambientColor = vec4(light.color, 1) * light.ambientIntensity;
    float diffuseFactor = dot(gIn.normal, -lightDirection);
    
    vec4 diffuseColor  = vec4(0, 0, 0, 0);
    vec4 specularColor = vec4(0, 0, 0, 0);
    
    if (diffuseFactor > 0) {
        diffuseColor = vec4(light.color, 1) * light.diffuseIntensity * diffuseFactor;
        
        vec3 lightReflect = normalize(reflect(lightDirection, gIn.normal));
        float specularFactor = dot(gIn.toCamera, lightReflect);
        specularFactor = pow(specularFactor, specularPower);
        
        if (specularFactor > 0) {
            specularColor = vec4(light.color, 1) * specularIntensity * specularFactor;
        }
    }
    
    vec4 totalLight = ambientColor + diffuseColor + specularColor;
    
    return totalLight;
}

void main() {
    vec4 totalLight = computeLight(directionalLight.base, directionalLight.direction);
    
    vec4 brush = texture(brushTexture, gIn.textureCoord);
    
    if (brush.x == 0) {
        vec2 inkCoord = (vec4(gIn.strokeOrientation)).xy;
        vec4 ink = texture(inkTexture, inkCoord);
        fragColor = ink;
    } else {
        fragColor = vec4(0, 0, 0, 0);
    }
}

