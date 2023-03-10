#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 3) in vec3 v_color;
layout(location = 2) in vec2 text_coord;
layout(location = 1) in vec3 v_normal;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// TODO: output values to fragment shader
out vec3 frag_color;
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_texture;


void main()
{
	// TODO: send output to fragment shader

	// TODO: compute gl_Position
	frag_color = v_color;
	frag_normal = v_normal + vec3(sin(Time), cos(Time), 0);
	frag_position = v_position;
	frag_texture = text_coord;

	gl_Position = Projection * View * Model * vec4(v_position, 1) + vec4(sin(Time), cos(Time) , 0, 0);
}