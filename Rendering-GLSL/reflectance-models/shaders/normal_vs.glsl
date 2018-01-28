#version 410

uniform mat4 view_mat;
uniform mat4 proj_mat;
uniform mat4 model_mat;

in vec3 position;
in vec3 normal;

out vec3 v_color;

void main() {
    v_color = (view_mat * vec4(normal, 0.0)).xyz;
    gl_Position =  proj_mat * view_mat * model_mat * vec4(position, 1.0);
}
