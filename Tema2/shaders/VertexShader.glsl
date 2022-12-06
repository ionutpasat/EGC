#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform vec3 car_pos;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;


void main()
{
	color = object_color;
	vec3 world_pos = vec3 (Model * vec4(v_position, 1.0f));
	world_pos.y = world_pos.y - (dot(car_pos - world_pos, car_pos - world_pos) * 0.01);
	gl_Position = Projection * View * vec4(world_pos, 1.0);
}
