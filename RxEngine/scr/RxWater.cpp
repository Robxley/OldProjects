//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#


#include "RxWater.h"

using namespace std;

RxWater::RxWater() :RxItem3D()
{
	type = RX_WATER_ITEM3D;
	waterPlane.set(vec3(0.0f,1.0f,0.0f),vec3(0.0f,0.0f,0.0f));
	color.set(0.7f,0.8f,1.0f,0.5f);
	sizeFBO.set(512,512);
	camera = NULL;
	vertFile = RX_WATER_VERT_FILE;
    fragFile = RX_WATER_FRAG_FILE;
	mode = 2;

}
RxWater::~RxWater()
{

}
void RxWater::Init(RxCamera *cam)
{
	
	reflectionFBO.Init();
	reflectionFBO.setViewport(sizeFBO);

	refractionFBO.Init();
	refractionFBO.setViewport(sizeFBO);
	camera = cam;

	/*initialisation du shader*/
	//charge le shader et création des variables uniformes en fonction du nombre de texture
	waterShader.Load(vertFile.data(), fragFile.data(), "RxWater");
		
	reflectMatrix.reflect(waterPlane.getEquation());
	

}

void RxWater::EnableReflection()
{
    mode = RX_REFLECT;

	glGetIntegerv( GL_VIEWPORT, viewportSaver );
	glGetFloatv(GL_COLOR_CLEAR_VALUE, clearcolorSaver);
	if(camera != NULL)
    {
		
        reflectionFBO.Enable();
		glClearColor(1.0f,1.0f,1.0f,0.0f);
		//glClearColor(color.r,color.g, color.b,color.a);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glViewport(0,0,sizeFBO.x,sizeFBO.y);

		//camera au dessus de l'eau
		glPushMatrix();
        if(camera->pos.y >  waterPlane.getOriginPoint().y)
        {
			glMultMatrixf(reflectMatrix.mat);//matrix de reflection	
            glClipPlane(GL_CLIP_PLANE0,waterPlane.getGlEquation());
            glEnable(GL_CLIP_PLANE0);
				

        }
    }
}

void RxWater::DisableReflection()
{
	if(camera != NULL )
    {
        glDisable(GL_CLIP_PLANE0);
        glPopMatrix();
		reflectionFBO.Disable();
		glClearColor(clearcolorSaver[0],clearcolorSaver[1],clearcolorSaver[2],clearcolorSaver[3]);
        glViewport(viewportSaver[0],viewportSaver[1],viewportSaver[2],viewportSaver[3]);
    }
}
void RxWater::EnableRefraction()
{
	mode = RX_REFLECT_AND_REFRACT;  //on a la refraction et reflection

	glGetIntegerv( GL_VIEWPORT, viewportSaver );
	glGetFloatv(GL_COLOR_CLEAR_VALUE, clearcolorSaver);
	if(camera != NULL)
    {
	
        refractionFBO.Enable();
		glClearColor(1.0f,1.0f,1.0f,0.0f);
		//glClearColor(color.r,color.g, color.b,color.a);
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glViewport(0,0,sizeFBO.x,sizeFBO.y);

		//camera au dessus de l'eau
		glPushMatrix();
     //   if(camera->pos.y >  waterPlane.getOriginPoint().y)
      /*  {

			RxPlane tmp;
			tmp.set(waterPlane.getNormal(),waterPlane.getOriginPoint());
			glClipPlane(GL_CLIP_PLANE0,tmp.getGlEquation());
			//glClipPlane(GL_CLIP_PLANE0,waterPlane.getGlEquation());
			glEnable(GL_CLIP_PLANE0);
        }*/
    }
}
void RxWater::DisableRefraction()
{
	if(camera != NULL )
    {
        glDisable(GL_CLIP_PLANE0);
		glPopMatrix();
        refractionFBO.Disable();
		glClearColor(clearcolorSaver[0],clearcolorSaver[1],clearcolorSaver[2],clearcolorSaver[3]);
        glViewport(viewportSaver[0],viewportSaver[1],viewportSaver[2],viewportSaver[3]);
    }
}
RxIDTexture RxWater::getReflectionTex()
{
	return reflectionFBO.getColor();
}
RxIDTexture RxWater::getRefractionTex()
{
	return refractionFBO.getColor();
}

void RxWater::setCamera(RxCamera *cam)
{
	camera = cam;
}

void RxWater::Draw()
{
	   // glPolygonOffset(5.0f,10.0f);
		//glEnable(GL_POLYGON_OFFSET_FILL);
		if(RxItem3D::GeometryOnlyFlag)
		{
				if(waterSurface.getNbFaces())
					waterSurface.Draw();
				else
					waterPlane.Draw();

				return;
		}

		glDisable(GL_LIGHTING);
	    if(mode == RX_TRANSPARENCY)
		{
						glEnable(GL_BLEND);
						glDisable(GL_TEXTURE_2D);
						glBindTexture (GL_TEXTURE_2D, noiseMap);
						
						glMatrixMode(GL_TEXTURE); 
						glPushMatrix();
						glTranslatef(wavetime,0.0f,0.0f);
							
							glMatrixMode(GL_MODELVIEW); 
							
							glColor4f(color.r,color.g, color.b,color.a);

							//glColor4f(1.0f,0.0f,1.0f,0.0f);
							//glTranslatef(0.0,-0.5,0.0);
							if(waterSurface.getNbFaces())
								waterSurface.Draw();
							else
								waterPlane.Draw();
							glMatrixMode(GL_TEXTURE); 
						glPopMatrix();
						glMatrixMode(GL_MODELVIEW); 
						
						
						//glEnable(GL_TEXTURE_2D);
						glDisable(GL_BLEND);
		}
		
	else if(mode== RX_REFLECT)
		{
			if(waterShader.Enable())
			{
					//transmission des uniforms au shader
					//glEnable(GL_TEXTURE_2D);

					glActiveTexture (GL_TEXTURE0+3);
					glBindTexture (GL_TEXTURE_2D,0 );
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_REFRACTION_MAP,3);

					glActiveTexture (GL_TEXTURE0+2);
					glBindTexture (GL_TEXTURE_2D,reflectionFBO.getColor() );
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_REFLECTION_MAP,2);

					glActiveTexture (GL_TEXTURE0+1);
					glBindTexture (GL_TEXTURE_2D, noiseMap);
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_NOISE_MAP,1);

					glActiveTexture (GL_TEXTURE0);
					glBindTexture (GL_TEXTURE_2D, normalMap);
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_NORMAL_MAP,0);



					waterShader.Send1f(RX_WATER_UNIFORM_NAME_TIME,wavetime);

					//on dessine le plan d'eau
					glEnable(GL_BLEND);
					glDisable(GL_LIGHTING);

						glPushMatrix();
							glColor4f(color.r,color.g, color.b,color.a);
							//glColor4f(1.0f,0.0f,1.0f,0.0f);
							//glTranslatef(0.0,-0.5,0.0);
							if(waterSurface.getNbFaces())
								waterSurface.Draw();
							else
								waterPlane.Draw();
						glPopMatrix();
					glDisable(GL_BLEND);
			}
		
				waterShader.Disable();
		}
		
	else if(mode == RX_REFLECT_AND_REFRACT)
		{
			if(waterShader.Enable())
			{
					//transmission des uniforms au shader
					///glEnable(GL_TEXTURE_2D);

					glActiveTexture (GL_TEXTURE0+3);
					glBindTexture (GL_TEXTURE_2D,refractionFBO.getColor() );
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_REFRACTION_MAP,3);

					glActiveTexture (GL_TEXTURE0+2);
					glBindTexture (GL_TEXTURE_2D,reflectionFBO.getColor() );
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_REFLECTION_MAP,2);

					glActiveTexture (GL_TEXTURE0+1);
					glBindTexture (GL_TEXTURE_2D, noiseMap);
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_NOISE_MAP,1);

					glActiveTexture (GL_TEXTURE0);
					glBindTexture (GL_TEXTURE_2D, normalMap);
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_NORMAL_MAP,0);



					waterShader.Send1f(RX_WATER_UNIFORM_NAME_TIME,wavetime);

					//on dessine le plan d'eau
				//	glEnable(GL_ALPHA_TEST);
				//	glAlphaFunc(GL_GREATER, 0.01f); //on garde que les pixels avec un alpha sup à zero
						glPushMatrix();
							glColor4f(color.r,color.g, color.b,color.a);
							//glColor4f(1.0f,0.0f,1.0f,0.0f);
							//glTranslatef(0.0,-0.5,0.0);
							if(waterSurface.getNbFaces())
								waterSurface.Draw();
							else
								waterPlane.Draw();
						glPopMatrix();
				//	glDisable(GL_ALPHA_TEST);
			}
		
				waterShader.Disable();
		}
	
	//glDisable(GL_POLYGON_OFFSET_FILL);
	mode = RX_TRANSPARENCY;
}
void RxWater::Draw(vec3 cpos, vec3 dir)
{
   // glPolygonOffset(5.0f,10.0f);
		//glEnable(GL_POLYGON_OFFSET_FILL);
		if(RxItem3D::GeometryOnlyFlag)
		{
				if(waterSurface.getNbFaces())
					waterSurface.Draw();
				else
					waterPlane.Draw();

				return;
		}

		glDisable(GL_LIGHTING);
	    if(mode == RX_TRANSPARENCY)
		{
						glEnable(GL_BLEND);
						glDisable(GL_TEXTURE_2D);
						glBindTexture (GL_TEXTURE_2D, noiseMap);
						
						glMatrixMode(GL_TEXTURE); 
						glPushMatrix();
						glTranslatef(wavetime,0.0f,0.0f);
							
							glMatrixMode(GL_MODELVIEW); 
							
							glColor4f(color.r,color.g, color.b,color.a);

							//glColor4f(1.0f,0.0f,1.0f,0.0f);
							//glTranslatef(0.0,-0.5,0.0);
							if(waterSurface.getNbFaces())
								waterSurface.Draw();
							else
								waterPlane.Draw();
							glMatrixMode(GL_TEXTURE); 
						glPopMatrix();
						glMatrixMode(GL_MODELVIEW); 
						
						
						//glEnable(GL_TEXTURE_2D);
						glDisable(GL_BLEND);
		}
		
	else if(mode== RX_REFLECT)
		{
			if(waterShader.Enable())
			{
					//transmission des uniforms au shader
					//glEnable(GL_TEXTURE_2D);

					glActiveTexture (GL_TEXTURE0+3);
					glBindTexture (GL_TEXTURE_2D,0 );
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_REFRACTION_MAP,3);

					glActiveTexture (GL_TEXTURE0+2);
					glBindTexture (GL_TEXTURE_2D,reflectionFBO.getColor() );
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_REFLECTION_MAP,2);

					glActiveTexture (GL_TEXTURE0+1);
					glBindTexture (GL_TEXTURE_2D, noiseMap);
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_NOISE_MAP,1);

					glActiveTexture (GL_TEXTURE0);
					glBindTexture (GL_TEXTURE_2D, normalMap);
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_NORMAL_MAP,0);



					waterShader.Send1f(RX_WATER_UNIFORM_NAME_TIME,wavetime);

					//on dessine le plan d'eau
					glEnable(GL_BLEND);
					glDisable(GL_LIGHTING);

						glPushMatrix();
							glColor4f(color.r,color.g, color.b,color.a);
							//glColor4f(1.0f,0.0f,1.0f,0.0f);
							//glTranslatef(0.0,-0.5,0.0);
							if(waterSurface.getNbFaces())
								waterSurface.Draw();
							else
								waterPlane.Draw();
						glPopMatrix();
					glDisable(GL_BLEND);
			}
		
				waterShader.Disable();
		}
		
	else if(mode == RX_REFLECT_AND_REFRACT)
		{
			if(waterShader.Enable())
			{
					//transmission des uniforms au shader
					///glEnable(GL_TEXTURE_2D);

					glActiveTexture (GL_TEXTURE0+3);
					glBindTexture (GL_TEXTURE_2D,refractionFBO.getColor() );
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_REFRACTION_MAP,3);

					glActiveTexture (GL_TEXTURE0+2);
					glBindTexture (GL_TEXTURE_2D,reflectionFBO.getColor() );
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_REFLECTION_MAP,2);

					glActiveTexture (GL_TEXTURE0+1);
					glBindTexture (GL_TEXTURE_2D, noiseMap);
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_NOISE_MAP,1);

					glActiveTexture (GL_TEXTURE0);
					glBindTexture (GL_TEXTURE_2D, normalMap);
					waterShader.Send1i(RX_WATER_UNIFORM_NAME_NORMAL_MAP,0);



					waterShader.Send1f(RX_WATER_UNIFORM_NAME_TIME,wavetime);

					//on dessine le plan d'eau
				//	glEnable(GL_ALPHA_TEST);
				//	glAlphaFunc(GL_GREATER, 0.01f); //on garde que les pixels avec un alpha sup à zero
						glPushMatrix();
							glColor4f(color.r,color.g, color.b,color.a);
							//glColor4f(1.0f,0.0f,1.0f,0.0f);
							//glTranslatef(0.0,-0.5,0.0);
							if(waterSurface.getNbFaces())
								waterSurface.Draw();
							else
								waterPlane.Draw();
						glPopMatrix();
				//	glDisable(GL_ALPHA_TEST);
			}
		
				waterShader.Disable();
		}
	
	//glDisable(GL_POLYGON_OFFSET_FILL);
	mode = RX_TRANSPARENCY;
}
void RxWater::setNoiseMap(RxIDTexture id)
{
	noiseMap = id;
}
void RxWater::setNormalMap(RxIDTexture id)
{
	normalMap = id;
}
void RxWater::setTime(float t)
{
	wavetime = t;
}
/*void RxWater::setSize(float w, float h)
{
	size.set(w,h);
}
void RxWater::setSize(vec2 s)
{
   size = s;
}
void RxWater::setPosition(vec3 p)
{
	position = p;
}
void RxWater::setPosition(float x, float y, float z)
{
	position.set(x,y,z);
}*/
void RxWater::setWaterPlane(RxPlane wp)
{
	waterPlane = wp;
	reflectMatrix.reflect(waterPlane.getEquation());
}
void RxWater::setWaterColor(float r, float g, float b, float a)
{
	color.set(r,g,b,a);
}
void RxWater::setWaterColor(vec4 c)
{
	color = c;
}
vec4 RxWater::getWaterColor()
{
	return color;
}
RxPlane RxWater::getWaterPlane()
{
	return waterPlane;
}