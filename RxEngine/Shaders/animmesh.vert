#version 330



in vec3 in_Vertex;
in vec3 in_Color;
in vec2 in_Coord0;
in vec3 in_Data0;
in ivec4 in_Data1; //BoneIDs
in vec4 in_Data2; // bone Weights

out vec3 color;
out vec2 texCoord;

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 transform;

uniform mat4 bonesMatrix[64];


void main()
{
    color = in_Color;
	texCoord = in_Coord0;
	vec4 newPos = vec4(in_Vertex, 1.0);

	if( in_Data2 !=vec4(0.0,0.0,0.0,0.0))
	{
		mat4 boneTransform = bonesMatrix[int(in_Data1[0])] * in_Data2[0];
		boneTransform     += bonesMatrix[int(in_Data1[1])] * in_Data2[1];
		boneTransform     += bonesMatrix[int(in_Data1[2])] * in_Data2[2];
		boneTransform     += bonesMatrix[int(in_Data1[3])] * in_Data2[3];
		newPos =  boneTransform*newPos;
	}



    gl_Position = projection * modelview *transform*newPos;
}
