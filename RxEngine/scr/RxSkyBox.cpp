


#include "RxSkyBox.h"


RxSkyBox::RxSkyBox( RxIDTexture _top ,   RxIDTexture _bottom,
				    RxIDTexture _front , RxIDTexture _back,
				    RxIDTexture _right , RxIDTexture _left,
					const char *_vertShader, const char *_fragShader,
					glm::vec4 _color,
					float _width, float _height , float _length )
					: width(_width), height(_height), length(_length),
					 vertexShader(_vertShader), fragmentShader(_fragShader),
					 color(_color)
{
	setSkyBoxTexture(_top,_bottom,_front,_back,_right,_left);
}


RxSkyBox::~RxSkyBox(){skyBoxVBO.Destroy();}


void RxSkyBox::Init()
{
	skyBoxVBO.Destroy();
	float v[] =
	{
		 //back			
		 0.0f,  0.0f, 0.0f,
		 RX_SKYBOX_WIDTH, 0.0f, 0.0f,
		 0.0f, RX_SKYBOX_HEIGHT, 0.0f,
		 RX_SKYBOX_WIDTH, RX_SKYBOX_HEIGHT, 0.0f,
		 
		 //front
		 0.0f, 0.0f, RX_SKYBOX_LENGHT,
		 0.0f, RX_SKYBOX_HEIGHT, RX_SKYBOX_LENGHT,
		 RX_SKYBOX_WIDTH, 0.0f, RX_SKYBOX_LENGHT,
		 RX_SKYBOX_WIDTH, RX_SKYBOX_HEIGHT, RX_SKYBOX_LENGHT,
		 
		 //bottom
		 0.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, RX_SKYBOX_LENGHT,
		 RX_SKYBOX_WIDTH, 0.0f, 0.0f,
		 RX_SKYBOX_WIDTH, 0.0f, RX_SKYBOX_LENGHT,
				
		 //top
		 0.0f, RX_SKYBOX_HEIGHT, 0.0f,
		 RX_SKYBOX_WIDTH, RX_SKYBOX_HEIGHT, 0.0f,
		 0.0f, RX_SKYBOX_HEIGHT, RX_SKYBOX_LENGHT,
		 RX_SKYBOX_WIDTH, RX_SKYBOX_HEIGHT, RX_SKYBOX_LENGHT,
		
		 //right
		 0.0f, 0.0f, 0.0f,
		 0.0f, RX_SKYBOX_HEIGHT, 0.0f,
		 0.0f, 0.0f, RX_SKYBOX_LENGHT,
		 0.0f, RX_SKYBOX_HEIGHT, RX_SKYBOX_LENGHT,
				
		 //left
		 RX_SKYBOX_WIDTH, 0.0f, 0.0f,
		 RX_SKYBOX_WIDTH, 0.0f, RX_SKYBOX_LENGHT,
		 RX_SKYBOX_WIDTH, RX_SKYBOX_HEIGHT, 0.0f,
		 RX_SKYBOX_WIDTH, RX_SKYBOX_HEIGHT, RX_SKYBOX_LENGHT

	};

	float vt[] =
	{
				
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f, 

			
		1.0f, 0.0f,
		1.0f, 1.0f, 
		0.0f, 0.0f,
		0.0f, 1.0f, 	
				
		1.0f, 0.0f,
		1.0f, 1.0f, 
		0.0f, 0.0f,
		0.0f, 1.0f, 	
					
		1.0f, 1.0f, 
		0.0f, 1.0f, 
		1.0f, 0.0f,
		0.0f, 0.0f,	
					
		1.0f, 0.0f,
		1.0f, 1.0f, 
		0.0f, 0.0f,
		0.0f, 1.0f,
					
		0.0f, 0.0f,
		1.0f, 0.0f,
		0.0f, 1.0f, 
		1.0f, 1.0f

	};


	std::vector<glm::vec3>	tPosition;	
	std::vector<glm::vec2>	tTexcoord;	
	

	unsigned int i;
	for(i = 0; i < 24; i++)
	{
		tPosition.push_back(glm::vec3(v[i*3]*width,v[i*3+1]*height,v[i*3+2]*length)-glm::vec3(width*0.5f,height*0.5f,length*0.5f));
		tTexcoord.push_back(glm::vec2(vt[i*2],vt[i*2+1]));
	}

	skyBoxVBO.AddArrayBufferData(RX_VERTEX_ATTRIB_INDEX, tPosition);
	skyBoxVBO.AddArrayBufferData(RX_COORD0_ATTRIB_INDEX, tTexcoord);

	if(!skyBoxVBO.Create(GL_STATIC_DRAW))
		RXLOGGERERROR("[RxSkyBox][Init] VBO Fail");


	skyBoxShader.Load(vertexShader.c_str(), fragmentShader.c_str());


}
void RxSkyBox::SetSize(float _width, float _height, float _length)
{
	width = _width;
	height = _height;
	length = _length;

}

void RxSkyBox::setSkyBoxTexture( RxIDTexture _top,   RxIDTexture _bottom,
								 RxIDTexture _front, RxIDTexture _back,
								 RxIDTexture _right, RxIDTexture _left)
{
	top = _top;
	bottom = _bottom;
	front = _front;
	back = _back;
	right = _right;
	left = _left;

	glBindTexture(GL_TEXTURE_2D,top);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D,bottom);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D,front);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D,back);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D,right);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D,left);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void RxSkyBox::Draw(const glm::mat4 &projection, const glm::mat4 &modelview)
{

	glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
	

	glActiveTexture(GL_TEXTURE0);
	skyBoxShader.Enable();
	skyBoxShader.Send1i("textureSampler", 0);
	skyBoxShader.Send4f("color", color);
	skyBoxShader.SendMat4f("modelview",GL_FALSE, modelview);
	skyBoxShader.SendMat4f("projection",GL_FALSE, projection);


	skyBoxVBO.Enable();
  

		glBindTexture(GL_TEXTURE_2D,back);
		glDrawArrays(GL_TRIANGLE_STRIP,0,4); //back

		glBindTexture(GL_TEXTURE_2D,front);
		glDrawArrays(GL_TRIANGLE_STRIP,4,4); //front

		glBindTexture(GL_TEXTURE_2D,bottom);
		glDrawArrays(GL_TRIANGLE_STRIP,8,4); //bottom

		glBindTexture(GL_TEXTURE_2D,top);
		glDrawArrays(GL_TRIANGLE_STRIP,12,4);//top

		glBindTexture(GL_TEXTURE_2D,right);
		glDrawArrays(GL_TRIANGLE_STRIP,16,4);//right

		glBindTexture(GL_TEXTURE_2D,left);
		glDrawArrays(GL_TRIANGLE_STRIP,20,4);//left


	skyBoxVBO.Disable();
	skyBoxShader.Disable();

    glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);


}

void RxSkyBox::setColor(glm::vec4& _color)
{
	color = _color;
}