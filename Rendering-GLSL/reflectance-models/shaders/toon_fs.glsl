#version 410

in vec3 v_position;
in vec3 v_normal;
in vec3 v_color;

out vec4 f_color;

uniform mat4 view_mat;
uniform mat4 model_mat;

const float kd = 1.0;
const float specular_exponent = 6;

vec3 diffuse_color = v_color;
vec3 light_position_world  = vec3(0.0, 0.0, 40.0);

void main() {
    vec3 light_position = (view_mat * vec4(light_position_world, 1.0)).xyz;
    vec3 surface_to_light = normalize(light_position - v_position);
    float cosine = dot(surface_to_light, v_normal);
    vec4 diff;

    if (cosine > 0.95) {
        diff = vec4(1.0, 1.0, 1.0,1.0);
    } else if (cosine > 0.5) {
        diff = vec4(0.8, 0.8, 0.8,1.0);
    } else if (cosine > 0.25) {
        diff = vec4(0.6, 0.6, 0.6, 1.0);
    } else {
        diff = vec4(0.2, 0.2, 0.2, 1.0);
    }

    f_color = kd * diff * vec4(diffuse_color, 1.0);
}
