#version 430

in vec4 color;
in vec2 TexCoord;

 uniform sampler2D ourTexture;

out vec4 out_color;
void main()
{
//    out_color = color;
     out_color = texture(ourTexture, TexCoord) * color;
}
