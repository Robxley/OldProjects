#version 140

in vec3 color;
in vec2 texCoord;
out vec4 out_Color;


uniform sampler2D textureSampler;
 
void main()
{
    vec4 color_t = texture2D( textureSampler, texCoord );
    //out_Color = (vec4(color,1.0) + color_t)*0.5f;
	//out_Color = vec4(color,1.0);
	out_Color = color_t;
}
