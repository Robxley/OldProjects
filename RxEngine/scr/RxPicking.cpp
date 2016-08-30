

#include "RxPicking.h"



RxPicking::RxPicking(unsigned int precision_ratio)
{
	fbo.Init(); //récupération automatique de la taille viewport et initialisation
	realSize = fbo.getSize();
	fbo.setViewport(realSize.x/precision_ratio,realSize.y/precision_ratio);
	fbo_ratio = precision_ratio;
	shader.Load(RX_PICKING_VERT,RX_PICKING_FRAG,"picking");
	shader.CreateUniformVar("key");
}

void RxPicking::setPrecisionRatio(unsigned int ratio)
{
	fbo.setViewport(realSize.x/ratio,realSize.y/ratio);
	fbo_ratio = ratio;
}


//ivec3 RxPicking::Key2Color(unsigned int _key)
//{
//	if(_key <=255)
//	{
//		return ivec3(_key,0,0);
//	}
//	else if(_key <= 65535) //256*256-1
//	{
//		unsigned int g = _key/256;
//		unsigned int r = _key-g*256;
//		return ivec3(r,g,0);
//	}
//	else if(_key <= 16777215)//256*256*256-1
//	{
//		unsigned int b = _key/(65536);
//		unsigned int g = (_key - b*65536) / 256;
//		unsigned int r = _key - (b*65536+g*256);
//		return ivec3(r,g,b);
//	}
//
//	return ivec3(0,0,0);
//}
ivec3 RxPicking::Key2Color(unsigned int _key)
{
	//long r = _key & 255;
	//long g = (_key >> 8) & 255;
	//long b = (_key >> 16) & 255;
	//return ivec3(r,g,b);
	return ivec3(_key & 255, (_key >> 8) & 255, (_key >> 16) & 255);
}


unsigned int RxPicking::Color2key(ivec3 _color)
{
	//return _color.red + _color.green*256 + _color.blue*65536;
	return (_color.red | (_color.green<<8) | (_color.blue<<16));
}


unsigned int RxPicking::ReadKeyInBuffer(unsigned int x, unsigned int y)
{

	if(fbo_ratio > 1)
	{
		x /=fbo_ratio;
		y /=fbo_ratio;
	}

	GLubyte pixel[3];

	GLuint idFrame = fbo.getFrameBuffer();
	glBindFramebuffer(GL_FRAMEBUFFER, idFrame);
	glReadPixels(x,y,1,1,GL_RGB,GL_UNSIGNED_BYTE,pixel);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return Color2key(ivec3(pixel[0],pixel[1],pixel[2]));
	
}

unsigned int RxPicking::ReadKeyInBuffer(ivec2 position)
{
	return ReadKeyInBuffer(position.x,position.y);
}

void RxPicking::BeginPickingMod()
{
	glShadeModel(GL_FLAT); //pas de mélange de couleur
	glDisable(GL_TEXTURE_2D); //pas de texture
	glDisable(GL_LIGHTING); //pas de lumière
	glDisable(GL_BLEND);
	glGetFloatv(GL_COLOR_CLEAR_VALUE,save_clear_color); //sauvegarde de la valeur d'effacement
	glClearColor(clear_color_picking.r, clear_color_picking.g,clear_color_picking.b,clear_color_picking.a);

	fbo.Enable();
	shader.Enable();
}
void RxPicking::EndPigkingMod()
{
	shader.Disable();
	fbo.Disable();
	glShadeModel(GL_SMOOTH);
	glClearColor(save_clear_color.r,save_clear_color.g,save_clear_color.b,save_clear_color.a);
}


RxFBO *RxPicking::getFBO()
{
	return &fbo;
}



/////////////////////////////////////////////////////////////////////////////////
/////////////////  RxPickedItem  //////////////////////////////////////////////// 


RxPickedItem::RxPickedItem(): pickable(0),picked(0),data(NULL)
{
}
RxPickedItem::~RxPickedItem()
{
}
void RxPickedItem3D::Draw()
{
	glPushMatrix();
		glMultMatrixf(transform);
		pItem3D->Draw();
	glPopMatrix();
}
void RxPickedItem3D::CallBackFct()
{
	pItem3D->getMaterial().color.a = 0.5f;
}
void RxPickedActor::Draw()
{
	pActor->Draw();
}
void RxPickedActor::CallBackFct()
{
	pActor->getItem3D()->getMaterial().color.a = 0.5f;
}
RxPickedItem3D::RxPickedItem3D():pItem3D(NULL)
{}
RxPickedActor::RxPickedActor():pActor(NULL)
{}
/////////////////////////////////////////////////////////////////////////////////
/////////////////  RxPickingManager  //////////////////////////////////////////////// 
RxPickingManager::RxPickingManager(unsigned int precision_ratio): picker(precision_ratio)
{
	picker.clear_color_picking = vec4(1.0,1.0,1.0,0.0);
}

void RxPickingManager::AddPikedItem(RxPickedItem *pItem)
{
	tabPickedItem.push_back(pItem);
}

void RxPickingManager::DrawPickingMod(int mx, int my)
{
	    picker.BeginPickingMod();
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glMultMatrixf(modelViewMat);
		GLuint uid = picker.shader.getUniform("key");

			for(unsigned int i = 0; i < tabPickedItem.size(); i++)
			{
				    glUniform1i(uid,i); //offset +1 car la valeur du fond correspond au noir et de clé 0
				//	ivec3 colorkey = picker.Key2Color(i+1);
				//	glColor3ub(colorkey.red, colorkey.green, colorkey.blue);
					tabPickedItem[i]->Draw();
			}

	   
	      //relève l'état de la sourie (bouton enfoncé et position)

	   glPopMatrix();
	  // glFlush();
	   picker.EndPigkingMod();

	   pickedKey = picker.ReadKeyInBuffer(mx,my);
	   if(pickedKey >= 0 && pickedKey < tabPickedItem.size())
			tabPickedItem[pickedKey]->CallBackFct();
}

RxPickedItem *RxPickingManager::getWhoIsPicked()
{
	if(pickedKey >= 0 && pickedKey < tabPickedItem.size())
		return tabPickedItem[pickedKey];

	return NULL;
}
unsigned int RxPickingManager::getKey()
{
	return pickedKey;
}