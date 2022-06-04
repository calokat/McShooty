#version 300 es
layout (location=0) in vec3 in_position;
layout (location=2) in vec3 in_normal;
layout (location=4) in vec2 aTexCoord;
uniform vec4 in_color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;
out vec3 normal;
out vec3 worldPos;
out vec2 TexCoord;

void main()
{
    // gl_Position = vec4(in_position, 1);
    gl_Position = projection * view * model * vec4(in_position, 1);
    color = in_color;
    normal = mat3(transpose(inverse(model))) * in_normal;
    TexCoord = aTexCoord;
    worldPos = (model * vec4(in_position, 1)).xyz;
}
