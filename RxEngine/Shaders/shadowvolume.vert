#version 330

#define INFINITY 1000.0f //projection � l'infinit
#define EPSILON 0.01f //offset projection pr supprimer les artefacts d'ombre sur l'objet lui-m�me

in vec4 in_Vertex;

uniform vec4 light;
uniform float projected;

uniform mat4 transform;
uniform mat4 projection, modelview;

void main()
{

		   float value_proj = EPSILON;
		   //projection � l'infini forcer par "projected" ou par la composante w � 0
		   if(in_Vertex.w == 0.0f || projected >= 0.5f)
			value_proj = INFINITY;

		   //lumi�re ponctuelle � �mission sph�rique
           if(light.w == 1.0f)  //w = 1 : lumi�re ponctuelle
            {

                 vec4 vertex =  transform*in_Vertex;      //place le vertex
                 vec3 projection_light= normalize(vertex.xyz-light.xyz);
                 vertex = vertex - value_proj *vec4(projection_light,1.0f);
				 vertex.w = 1.0f;
                 gl_Position = projection * modelview* vertex;
                 //gl_ClipVertex = vertex;
            }
			//w = 0 : lumi�re directionnelle � rayon parall�le
            else         
            {

                vec3 vertex =  vec3(transform*in_Vertex)+value_proj*light.xyz;      //place le vertex
                gl_Position = projection *modelview* vec4(vertex,1.0f);  //projection � l'�cran
               // gl_ClipVertex = vertex;
            }

}
