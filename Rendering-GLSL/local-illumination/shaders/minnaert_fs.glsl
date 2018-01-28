#version 410

in vec3 v_position;
in vec3 v_normal;
in vec3 v_color;

out vec4 f_color;

uniform mat4 view_mat;
uniform mat4 model_mat;

const float ka = 0.1;
const float kd = 1.0;
const float factor = 1.5;

vec3 ambient_color = vec3(1.0, 1.0, 1.0);
vec3 diffuse_color = v_color;
vec3 light_position_world  = vec3(0.0, 0.0, 40.0);

void main() {
    ambient_color = ka * ambient_color;

    vec3 light_position = (view_mat * vec4(light_position_world, 1.0)).xyz;
    vec3 surface_to_light = normalize(light_position - v_position);
    float dot_prod = dot(surface_to_light, v_normal);
    dot_prod = max(0.0, dot_prod);
    vec3 eye = normalize(-(view_mat * vec4(0.0, 0.0, 0.0, 1.0)).xyz);
    float dot_prod_2 = dot(eye, v_normal);
    dot_prod_2 = max(0.0, dot_prod_2);
    diffuse_color = kd * diffuse_color * pow(dot_prod, factor + 1) * pow(1.0 - dot_prod_2, 1 - factor);

    f_color = vec4(ambient_color + diffuse_color, 1.0);
}
