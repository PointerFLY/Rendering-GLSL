#version 410

in SVertexOutput
{
    vec3 WorldPos;
    vec3 ToCamera;
    vec3 Normal;
    vec2 TexCoord;
    float StrokePressure;
    float StrokeOrientation;
} In;

out vec4 FragColor;

struct SLightBase
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct SDirectionalLight
{
    SLightBase Base;
    vec3 Direction;
};

uniform SDirectionalLight DirectionalLight;
uniform float SpecularIntensity;  // specular reflectivity coefficient
uniform float SpecularPower;  // shininess or phong exponent

uniform bool DiffuseTextureEnabled;
uniform sampler2D DiffuseTexture;

uniform sampler2D BrushTexture;
uniform sampler2D InkTexture;

vec4 computeLight(SLightBase light, vec3 lightDirection)
{
    vec4 ambientColor = vec4(light.Color, 1) * light.AmbientIntensity;
    float diffuseFactor = dot(In.Normal, -lightDirection);
    
    vec4 diffuseColor  = vec4(0, 0, 0, 0);
    vec4 specularColor = vec4(0, 0, 0, 0);
    
    if (diffuseFactor > 0)
    {
        diffuseColor = vec4(light.Color, 1) * light.DiffuseIntensity * diffuseFactor;
        
        vec3 lightReflect = normalize(reflect(lightDirection, In.Normal));
        float specularFactor = dot(In.ToCamera, lightReflect);
        specularFactor = pow(specularFactor, SpecularPower);
        
        if (specularFactor > 0)
        {
            specularColor = vec4(light.Color, 1) * SpecularIntensity * specularFactor;
        }
    }
    
    vec4 totalLight = ambientColor + diffuseColor + specularColor;
    
    return totalLight;
}

vec4 computeDirectionalLight()
{
    return computeLight(DirectionalLight.Base, DirectionalLight.Direction);
}

void main()
{
    vec4 totalLight = computeDirectionalLight();
    
    vec4 brush = texture(BrushTexture, In.TexCoord);
    
    if (brush.x == 0)
    {
        mat4 rotation = mat4(
                             cos(1.0), -sin(1.0), 0.0, 0.0,
                             sin(1.0),  cos(1.0), 0.0, 0.0,
                             0.0,            0.0, 1.0, 0.0,
                             0.0,            0.0, 0.0, 1.0 );
        vec2 inkCoord = (rotation * vec4(In.StrokeOrientation)).xy;
        
        vec4 ink = texture(InkTexture, inkCoord);
        
        FragColor = ink;
    }
    else
    {
        FragColor = vec4(0, 0, 0, 0);
    }
}
