#version 300 es
layout (location=0) in vec3 in_position;
layout (location=2) in vec3 in_normal;
layout (location=4) in vec2 aTexCoord;
layout (location=8) in vec3 in_tangent;
uniform vec4 in_color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 color;
out vec3 worldPos;
out vec2 TexCoord;
out mat3 TBN;
// out vec3 normal;
out vec3 tangent;

void main()
{
    // gl_Position = vec4(in_position, 1);
    gl_Position = projection * view * model * vec4(in_position, 1);
    color = in_color;
    // normal = in_normal;
    tangent = in_tangent;
    TexCoord = aTexCoord;
    worldPos = (model * vec4(in_position, 1)).xyz;
    vec3 biTangent = cross(in_tangent, in_normal);
    vec3 T = vec3(normalize(model * vec4(in_tangent, 0)));
    vec3 B = vec3(normalize(model * vec4(biTangent, 0)));
    vec3 N = vec3(normalize(model * vec4(in_normal, 0)));
    TBN = mat3(T, B, N);
}
