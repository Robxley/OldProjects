#version 330
#extension GL_EXT_texture_array : enable

uniform sampler2DArray arrayMap;


in vec3 data1;
in vec4 color;
in vec2 texCoord;
in vec3 normal;
flat in vec3 data0;


out vec4 out_Color;

void main (void)
{
	

    vec3 texCoord3 = vec3(texCoord,data0.x);
    vec4 color0 = texture2DArray(arrayMap,texCoord3);

    texCoord3 = vec3(texCoord, data0.y);
    vec4 color1 = texture2DArray(arrayMap,texCoord3);

    texCoord3 = vec3(texCoord, data0.z);
    vec4 color2 = texture2DArray(arrayMap,texCoord3);

	out_Color =  (data1.x*color0 + data1.y*color1+ data1.z*color2);
	
	//out_Color.r *= color.r;
	//out_Color.g *= color.g;
	//out_Color.b *= color.b;


}
