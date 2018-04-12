#version 410

layout(triangles) in;
layout(triangle_strip, max_vertices = 4) out;

in SVertexOutput
{
    vec3 WorldPos;
    vec3 ToCamera;
    vec3 Normal;
    float VN;
} In[3];

out SVertexOutput
{
    vec3 WorldPos;
    vec3 ToCamera;
    vec3 Normal;
    vec2 TexCoord;
    float StrokePressure;
    float StrokeOrientation;
} Out;

struct SVertex
{
    vec3 WorldPos;
    vec3 ToCamera;
    vec3 Normal;
    float VN;
    bool Valid;
};

uniform mat4 View;
uniform mat4 Projection;
uniform vec3 CameraPosition;

SVertex extractContour(SVertex v1, SVertex v2)
{
    SVertex result;
    
    vec3 n = v2.Normal - v1.Normal;
    vec3 v = v2.ToCamera - v1.ToCamera;
    
    float a = dot(n, v);
    float b = dot(v1.ToCamera, n) + dot(v1.Normal, v);
    float c = v1.VN;
    
    float delta = b * b - 4 * a * c;
    if (delta >= 0)
    {
        float lambda = (sqrt(delta) - b) / 2 / a;
        if (lambda >= 0 && lambda <= 1)
        {
            result.Valid = true;
            result.ToCamera = (1 - lambda) * v1.ToCamera + lambda * v2.ToCamera;
            result.WorldPos = CameraPosition - result.ToCamera;
            result.Normal = (1 - lambda) * v1.Normal + lambda * v2.Normal;
            
            return result;
        }
    }
    
    result.Valid = false;
    return result;
}

void emitContour(SVertex v1, SVertex v2)
{
    vec3 v = v2.WorldPos - v1.WorldPos;
    
    float vLen = length(v);
    //if (vLen < 0.00001) return;
    
    v /= vLen;
    
    vec4 vt4 = View * vec4(v, 0);
    
    Out.StrokePressure = 1 - abs(vt4.z) / length(vt4); //
    
    vt4 = Projection * vt4;
    vec2 vt2 = vt4.xy;
    Out.StrokeOrientation = acos(-vt2.x / length(vt2));
    
    vec3 normal = (v1.Normal + v2.Normal) / 2;
    normal = vec3(0,1,0);
    vec3 right = cross(v, normal);
    normal = normalize(cross(right, v));
    right = normalize(cross(v, normal));
    
    Out.Normal = normal;
    
    float halfWidth = 0.01;
    float halfHeight = 2 * vLen;
    
    Out.WorldPos = v1.WorldPos + right * halfWidth - v * halfHeight;
    Out.ToCamera = CameraPosition - Out.WorldPos;
    Out.TexCoord = vec2(0, 1);
    gl_Position = Projection * View * vec4(Out.WorldPos, 1);
    EmitVertex();
    
    Out.WorldPos = v1.WorldPos - right * halfWidth - v * halfHeight;
    Out.ToCamera = CameraPosition - Out.WorldPos;
    Out.TexCoord = vec2(0, 0);
    gl_Position = Projection * View * vec4(Out.WorldPos, 1);
    EmitVertex();
    
    Out.WorldPos = v2.WorldPos + right * halfWidth + v * halfHeight;
    Out.ToCamera = CameraPosition - Out.WorldPos;
    Out.TexCoord = vec2(1, 1);
    gl_Position = Projection * View * vec4(Out.WorldPos, 1);
    EmitVertex();
    
    Out.WorldPos = v2.WorldPos - right * halfWidth + v * halfHeight;
    Out.ToCamera = CameraPosition - Out.WorldPos;
    Out.TexCoord = vec2(1, 0);
    gl_Position = Projection * View * vec4(Out.WorldPos, 1);
    EmitVertex();
}

void main()
{
    SVertex vertices[3];
    
    for(int i = 0; i < 3; i++)
    {
        vertices[i].Normal = In[i].Normal;
        vertices[i].ToCamera = In[i].ToCamera;
        vertices[i].VN = In[i].VN;
    }
    
    SVertex contourV1;
    SVertex contourV2;
    contourV1.Valid = false;
    contourV2.Valid = false;
    
    if (vertices[0].VN * vertices[1].VN <= 0)
    {
        SVertex result = extractContour(vertices[0], vertices[1]);
        contourV1 = result;
    }
    if (vertices[0].VN * vertices[2].VN <= 0)
    {
        SVertex result = extractContour(vertices[0], vertices[2]);
        if (contourV1.Valid) contourV2 = result;
        else contourV1 = result;
    }
    if (vertices[1].VN * vertices[2].VN <= 0)
    {
        SVertex result = extractContour(vertices[1], vertices[2]);
        contourV2 = result;
    }
    
    if (contourV1.Valid && contourV2.Valid)
    {
        emitContour(contourV1, contourV2);
    }
    
    EndPrimitive();
}
