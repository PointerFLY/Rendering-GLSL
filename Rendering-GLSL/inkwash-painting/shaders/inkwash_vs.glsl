#version 410

in vec3 position;
in vec3 normal;
in vec2 textureCoord;

out SVertexOutput
{
    vec3 WorldPos;
    vec3 ToCamera;
    vec3 Normal;
    float VN;
} Out;

uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 CameraPosition;

void main()
{
    gl_Position = Projection * View * World * vec4(position, 1.0);
    Out.WorldPos = (World * vec4(position, 1.0)).xyz;
    Out.ToCamera = normalize(CameraPosition - Out.WorldPos);
    Out.Normal = (normalize(World * vec4(normal, 0.0))).xyz;
    Out.VN = dot(Out.ToCamera, Out.Normal);
}
