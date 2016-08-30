#version 330
#extension GL_EXT_texture_array : enable

uniform sampler2DArray arrayMap;

in vec3 data0;
in vec3 color;
in vec2 texCoord;
in vec3 normal;

out vec4 out_Color;

void main (void)
{


    vec3 texCoord3 = vec3(texCoord,0);
    vec4 color0 = texture2DArray(arrayMap,texCoord3);

    texCoord3 = vec3(texCoord, 1);
    vec4 color1 = texture2DArray(arrayMap,texCoord3);

    texCoord3 = vec3(texCoord, 2);
    vec4 color2 = texture2DArray(arrayMap,texCoord3);

	out_Color =  color.r*color0 + color.g*color1+ color.b*color2;

	out_Color.a = 1.0;

}
