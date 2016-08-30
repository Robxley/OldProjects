
// Vertex Attributes

attribute vec3 in_Vertex;
attribute vec2 in_Coord0;


// Uniforms

uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 transform;

// Varying

varying vec2 texCoord;

void main()
{
   gl_Position = projection * modelview * transform* vec4(in_Vertex, 1.0);   
   texCoord = in_Coord0;
}