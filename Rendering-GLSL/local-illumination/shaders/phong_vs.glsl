#version 410

in vec3 position;
in vec3 normal;

out vec3 v_position;
out vec3 v_normal;
out vec3 v_color;
out vec3 v_eye;

uniform mat4 proj_mat;
uniform mat4 view_mat;
uniform mat4 model_mat;

void main() {
    mat4 view_model_mat = view_mat * model_mat;
    v_position = (view_model_mat * vec4(position, 1.0)).xyz;
    v_normal = (view_model_mat * vec4(normal, 0.0)).xyz;
    v_color = v_normal;
    gl_Position = proj_mat * view_model_mat * vec4(position, 1.0);
}
