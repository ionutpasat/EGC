#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform vec3 object_color;
uniform int light_count;
uniform vec3 light_dir[2];
uniform vec3 light_pos[2];
uniform int spotlights[2];
uniform float cut_off_angles[2];

// Output
layout(location = 0) out vec4 out_color;

vec4 point_light_contribution(int currentLightSource)
{
	vec3 N = normalize(world_normal);
    vec3 L = normalize(light_pos[currentLightSource] - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    float ambient_light = material_kd * 0.25;
    float diffuse_light = material_kd * max(dot(normalize(N), L), 0);
    float specular_light = 0;
    float d = distance(light_pos[currentLightSource] , world_position);
    float factor_atenuare = 1 / (d*d + 1);
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(normalize(N), H), 0), material_shininess);
    }
    float light = ambient_light;
    if (spotlights[currentLightSource] == 1) {
        float spotlight = dot(-L, light_dir[currentLightSource]);
        float spotlight_limit = cos(radians(cut_off_angles[currentLightSource]));

        if (spotlight > spotlight_limit) {
            float linear_att = (spotlight - spotlight_limit) / (1 - spotlight_limit);
            float light_attfactor = pow(linear_att, 2);
            light = ambient_light + light_attfactor * factor_atenuare * (diffuse_light + specular_light);
        }
    } 
    else {
        light = ambient_light + factor_atenuare * (diffuse_light + specular_light);
    }
	return vec4 (object_color * light, 1);
}

void main()
{
    for (int i = 0; i < 2; i++) {
        vec4 color = point_light_contribution(i);
        out_color += color;
    }
}
