#version 430
layout (location=0) in vec3 in_position;
 layout (location=2) in vec2 aTexCoord;
// layout (location=2) in vec4 in_color;
uniform vec4 in_color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;
 out vec2 TexCoord;
void main()
{
    // gl_Position = vec4(in_position, 1);
    gl_Position = projection * view * model * vec4(in_position, 1);
    color = in_color;
	TexCoord = aTexCoord;
}
