#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int mixT;
uniform float elapsedTime;
// TODO(student): Declare various other uniforms

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec4 color1;
    vec4 color2;
    if (elapsedTime > 0) {
        color1 = texture2D(texture_1, texcoord + vec2(0, elapsedTime / 10));
        color2 = texture2D(texture_2, texcoord);
    } else {
        color1 = texture2D(texture_1, texcoord);
        color2 = texture2D(texture_2, texcoord);
    }

    if (mixT == 1) {
        out_color = mix(color1, color2, 0.5f);
    } else {
        out_color = color1;
    }
    if (out_color.a < 0.5f) {
        discard;
    }
    //out_color = vec4(1);

}
