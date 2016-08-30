#version 330

in vec2 texCoord;
out vec4 out_Color;

uniform sampler2D textureSampler;
uniform vec4 uColor;

void main()
{
    vec4 color_t = texture2D( textureSampler, texCoord );
	color_t.r *= uColor.r;
	color_t.g *= uColor.g;
	color_t.b *= uColor.b;
	color_t.a *= uColor.a;
    out_Color = color_t;
}
