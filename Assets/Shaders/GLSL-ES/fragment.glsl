#version 300 es
precision mediump float;

in vec4 color;
in vec2 TexCoord;

uniform sampler2D ourTexture;

out vec4 out_color;


void main()
{
//    gl_FragColor = color;
     out_color = texture(ourTexture, TexCoord) * color;
}
