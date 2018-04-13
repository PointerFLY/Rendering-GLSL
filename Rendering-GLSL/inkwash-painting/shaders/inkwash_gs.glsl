#version 410

struct Vertex {
    vec3 worldPosition;
    vec3 toCamera;
    vec3 normal;
    float vn;
    bool valid;
};

layout(triangles) in;
layout(triangle_strip, max_vertices = 4) out;

in VertexOutput {
    vec3 worldPosition;
    vec3 toCamera;
    vec3 normal;
    float vn;
} vIn[3];

uniform mat4 viewMat;
uniform mat4 projMat;
uniform vec3 cameraPosition;

out GeometryOutput {
    vec3 worldPosition;
    vec3 toCamera;
    vec3 normal;
    vec2 textureCoord;
    float strokePressure;
    float strokeOrientation;
} gOut;

Vertex extractContour(Vertex v1, Vertex v2) {
    Vertex result;
    
    vec3 n = v2.normal - v1.normal;
    vec3 v = v2.toCamera - v1.toCamera;
    
    float a = dot(n, v);
    float b = dot(v1.toCamera, n) + dot(v1.normal, v);
    float c = v1.vn;
    
    float delta = b * b - 4 * a * c;
    if (delta >= 0) {
        float lambda = (sqrt(delta) - b) / 2 / a;
        if (lambda >= 0 && lambda <= 1) {
            result.valid = true;
            result.toCamera = (1 - lambda) * v1.toCamera + lambda * v2.toCamera;
            result.worldPosition = cameraPosition - result.toCamera;
            result.normal = (1 - lambda) * v1.normal + lambda * v2.normal;
            
            return result;
        }
    }
    
    result.valid = false;
    return result;
}

void emitContour(Vertex v1, Vertex v2) {
    vec3 v = v2.worldPosition - v1.worldPosition;
    
    float vLen = length(v);
    v /= vLen;
    
    vec4 vt4 = viewMat * vec4(v, 0);
    
    gOut.strokePressure = 1 - abs(vt4.z) / length(vt4);
    
    vt4 = projMat * vt4;
    vec2 vt2 = vt4.xy;
    gOut.strokeOrientation = acos(-vt2.x / length(vt2));
    
    vec3 normal = (v1.normal + v2.normal) / 2;
    normal = vec3(0, 1, 0);
    vec3 right = cross(v, normal);
    normal = normalize(cross(right, v));
    right = normalize(cross(v, normal));
    
    gOut.normal = normal;
    
    float halfWidth = 0.0025;
    float halfHeight = 2.0 * vLen;

    gOut.worldPosition = v1.worldPosition + right * halfWidth - v * halfHeight;
    gOut.toCamera = cameraPosition - gOut.worldPosition;
    gOut.textureCoord = vec2(0, 1);
    gl_Position = projMat * viewMat * vec4(gOut.worldPosition, 1);
    EmitVertex();
    
    gOut.worldPosition = v1.worldPosition - right * halfWidth - v * halfHeight;
    gOut.toCamera = cameraPosition - gOut.worldPosition;
    gOut.textureCoord = vec2(0, 0);
    gl_Position = projMat * viewMat * vec4(gOut.worldPosition, 1);
    EmitVertex();
    
    gOut.worldPosition = v2.worldPosition + right * halfWidth + v * halfHeight;
    gOut.toCamera = cameraPosition - gOut.worldPosition;
    gOut.textureCoord = vec2(1, 1);
    gl_Position = projMat * viewMat * vec4(gOut.worldPosition, 1);
    EmitVertex();
    
    gOut.worldPosition = v2.worldPosition - right * halfWidth + v * halfHeight;
    gOut.toCamera = cameraPosition - gOut.worldPosition;
    gOut.textureCoord = vec2(1, 0);
    gl_Position = projMat * viewMat * vec4(gOut.worldPosition, 1);
    EmitVertex();
}

void main() {
    Vertex vertices[3];
    
    for (int i = 0; i < 3; i++) {
        vertices[i].normal = vIn[i].normal;
        vertices[i].toCamera = vIn[i].toCamera;
        vertices[i].vn = vIn[i].vn;
    }
    
    Vertex contourV1;
    Vertex contourV2;
    contourV1.valid = false;
    contourV2.valid = false;
    
    if (vertices[0].vn * vertices[1].vn <= 0) {
        Vertex result = extractContour(vertices[0], vertices[1]);
        contourV1 = result;
    }
    if (vertices[0].vn * vertices[2].vn <= 0) {
        Vertex result = extractContour(vertices[0], vertices[2]);
        if (contourV1.valid) {
            contourV2 = result;
        } else {
            contourV1 = result;
        }
    }
    if (vertices[1].vn * vertices[2].vn <= 0) {
        Vertex result = extractContour(vertices[1], vertices[2]);
        contourV2 = result;
    }
    if (contourV1.valid && contourV2.valid) {
        emitContour(contourV1, contourV2);
    }
    
    EndPrimitive();
}
