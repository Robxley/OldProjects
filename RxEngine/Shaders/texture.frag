#version 330

in vec3 color;
in vec2 texCoord;
out vec4 out_Color;


uniform sampler2D textureSampler;
 
void main()
{
    vec4 color_t = texture2D( textureSampler, texCoord );
	
	if(color.r > 0) color_t.r*=color.r;
	if(color.g > 0) color_t.g*=color.g;
	if(color.b > 0) color_t.b*=color.b;

	if(color_t.r == 0) color_t.r=color.r;
	if(color_t.g == 0) color_t.g=color.g;
	if(color_t.b == 0) color_t.b=color.b;

	
	//color_t.a=0.5f;
    out_Color = color_t;

}
