#version 330


in vec2 texCoord;
out vec4 out_Color;


uniform sampler2D textureSampler;
uniform vec4 color; 

void main()
{
    vec4 color_t = texture2D( textureSampler, texCoord );
    out_Color = color_t;

}
