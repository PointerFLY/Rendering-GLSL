#version 410

in vec3 v_position;
in vec3 v_normal;
in vec3 v_color;
in mat4 v_view_model_mat;

out vec4 f_color;

uniform mat4 view_mat;
uniform mat4 model_mat;

const float ka = 0.1;
const float kd = 1.0;
const float ks = 1.0;
const float specular_exponent = 6;

vec3 ambient_color = vec3(1.0, 1.0, 1.0);
vec3 diffuse_color = v_color;
vec3 specular_color = vec3(1.0, 1.0, 1.0);
vec3 light_position_world  = vec3(0.0, 0.0, 40.0);

void main() {
    ambient_color = ambient_color * ka;

    vec3 light_position = (view_mat * vec4(light_position_world, 1.0)).xyz;
    vec3 surface_to_light = normalize(light_position - v_position);
    float dot_prod = dot(surface_to_light, v_normal);
    dot_prod = max(0.0, dot_prod);
    diffuse_color = diffuse_color * dot_prod * kd;

    vec3 reflection = normalize(reflect(-light_position, v_normal));
    vec3 eye = normalize(-(view_mat * vec4(0.0, 0.0, 0.0, 1.0)).xyz);
    dot_prod = dot(reflection, eye);
    dot_prod = max(0.0, dot_prod);
    float specular_factor = pow(dot_prod, specular_exponent);
    specular_color = specular_color * specular_factor * ks;

    f_color = vec4(ambient_color + diffuse_color + specular_color, 1.0);
}
