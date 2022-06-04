#version 300 es
layout (location=0) in vec3 in_position;
uniform vec4 in_color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;
void main()
{
    gl_Position = projection * view * model * vec4(in_position, 1);
    color = in_color;
}
