#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;


void main()
{
    // TODO: compute world space vectors
	vec3 world_normal = vec3 (Model * vec4(v_normal, 1));
	vec3 world_pos = vec3 (Model * vec4(v_position, 0));

	vec3 L = normalize(light_position - world_pos);
	vec3 V = normalize(eye_position - world_pos);

	vec3 N = normalize(world_normal);
	vec3 H = normalize(L + V); //vectorul median

	// TODO: define ambient light component
	float ambient_light = material_kd * 0.2;

	// TODO: compute diffuse light component
	float diffuse_light = material_kd * max (dot(N, L), 0);

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		vec3 R = reflect(-L, N);
		specular_light = max(pow(dot(V, R), material_shininess), 0);
	}

	// TODO: compute light
	float light = diffuse_light + ambient_light + specular_light;
	
	// TODO: send color light output to fragment shader
	float att_factor = 1 / (pow(distance(light_position, world_pos), 2) + 1);
	color = object_color * (ambient_light + att_factor * (diffuse_light + specular_light));
	
	

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
