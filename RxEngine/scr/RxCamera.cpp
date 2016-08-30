//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#include "RxCamera.h"


RxCamera::RxCamera():
	pos(0.0f),dir(0.0f,0.0f,1.0f),top(0.0f,1.0f, 0.0f),
	moveSpeed(RX_CAMERA_MOVE_SPEED), rotationSpeed(RX_CAMERA_ROTATION_SPEED), normalisedSpeed(RX_CAMERA_DEFAULT_FPS), enable(RX_CAMERA_USE),
	longitude(0.0f), latitude(0.0f), elapseTime(NULL), sdlWindowID(NULL)
{
     //default code SDL /ASCII
     KEY_VALUES[RX_UP_KEY]     = SDLK_UP;
     KEY_VALUES[RX_DOWN_KEY]   = SDLK_DOWN;
     KEY_VALUES[RX_LEFT_KEY]   = SDLK_LEFT;
     KEY_VALUES[RX_RIGHT_KEY]  = SDLK_RIGHT;
}
RxCamera::RxCamera(float p_x, float p_y, float p_z, float v_x, float v_y, float v_z):
	pos(p_x,p_y,p_z), dir(v_x, v_y, v_z), top(0.0f, 1.0f, 0.0f),
	moveSpeed(RX_CAMERA_MOVE_SPEED), rotationSpeed(RX_CAMERA_ROTATION_SPEED), normalisedSpeed(RX_CAMERA_DEFAULT_FPS), enable(RX_CAMERA_USE),
	longitude(0.0f), latitude(0.0f), elapseTime(NULL), sdlWindowID(NULL)
{
     //default code SDL /ASCII
     KEY_VALUES[RX_UP_KEY]     = SDLK_UP;
     KEY_VALUES[RX_DOWN_KEY]   = SDLK_DOWN;
     KEY_VALUES[RX_LEFT_KEY]   = SDLK_LEFT;
     KEY_VALUES[RX_RIGHT_KEY]  = SDLK_RIGHT;


}
RxCamera::RxCamera(glm::vec3 _pos, glm::vec3 _dir, glm::vec3 _top):
	pos(_pos), dir(_dir), top(_top),
	moveSpeed(RX_CAMERA_MOVE_SPEED), rotationSpeed(RX_CAMERA_ROTATION_SPEED), normalisedSpeed(RX_CAMERA_DEFAULT_FPS), enable(RX_CAMERA_USE),
	longitude(0.0f), latitude(0.0f), elapseTime(NULL), sdlWindowID(NULL)
{

     //default code SDL /ASCII
     KEY_VALUES[RX_UP_KEY]     = SDLK_UP;
     KEY_VALUES[RX_DOWN_KEY]   = SDLK_DOWN;
     KEY_VALUES[RX_LEFT_KEY]   = SDLK_LEFT;
     KEY_VALUES[RX_RIGHT_KEY]  = SDLK_RIGHT;
}
float RxCamera::FpsNormalizer()
{
    if (elapseTime == NULL) return normalisedSpeed;
 
	return (float)(*elapseTime);
}
void RxCamera::setPosition(float p_x, float p_y, float p_z)
{ pos.x = p_x; pos.y = p_y; pos.z = p_z; }
void RxCamera::setPosition(glm::vec3 _pos){pos = _pos;}
void RxCamera::setTop(float t_x, float t_y, float t_z)
{
	top.x = t_x; top.y = t_y; top.z = t_z;
}

void RxCamera::Init(SDL_Window* w)
{

	if(w == NULL)
	{
		#ifdef _DEBUG
			RXLOGGERERROR("[RxCamera][Init] SDL_Window* w == NULL");
		#endif
		return;
	}
	sdlWindowID = w;
	setDirection(dir);
	CheckAngles();
}

void RxCamera::setDirection(glm::vec3 _dir)
{


	dir = _dir;

	dir = glm::normalize(dir);

    //on determine les angles correspondants au vecteur direction
    latitude = acos(dir.y);
    if (_dir.z < 0)
        longitude = -acos( dir.x/sqrt(dir.x*dir.x+dir.z*dir.z) ) ;
    else
        longitude = acos( dir.x/sqrt(dir.x*dir.x+dir.z*dir.z) );
    //on replace le curseur à sa position par défaut
	if(enable)
		SDL_WarpMouseInWindow(sdlWindowID, RX_MOUSE_DEFAUT_X, RX_MOUSE_DEFAUT_Y);
}
void RxCamera::setDirection(float d_x, float d_y, float d_z)
{
	setDirection(glm::vec3(d_x,d_y,d_z));
}

void RxCamera::setTop(glm::vec3 _top)
{top = _top;}


void RxCamera::setDirectionPoint(float dp_x, float dp_y, float dp_z)
{
	setDirection(glm::vec3(dp_x, dp_y, dp_z) - pos);
}
void RxCamera::setDirectionPoint(glm::vec3 _point)
{
	setDirection(_point-pos);
}
void RxCamera::CheckAngles()
{
    //evite de faire des dépacements d'intervalle
    if( longitude >= RX_2PI)
        longitude -= RX_2PI;
    else if( longitude <= 0 )
        longitude += RX_2PI;


    if( latitude > RX_PI-0.01f)
        latitude = RX_PI-0.01f;
    else if( latitude < 0.01f )
        latitude = 0.01f;
}
void RxCamera::ComputeAngles()
{
	    int mouseX,mouseY;

        //on relève la position de la souris
        SDL_GetMouseState(&mouseX, &mouseY);
        
       	

        //calcul du deplacement de la sourie par rapport à position par défaut
		 mouseX-=RX_MOUSE_DEFAUT_X;
         mouseY-=RX_MOUSE_DEFAUT_Y;

		//float dt = FpsNormalizer();

        //nouveaux angles
        //latitude  += mouseY * v_rot * dt;
        //longitude += mouseX * v_rot * dt;
		if(mouseY || mouseX)
		{
			latitude  += mouseY * rotationSpeed ;
			longitude += mouseX * rotationSpeed ;

			//on replace le curseur à sa position par défaut
			SDL_WarpMouseInWindow(sdlWindowID, RX_MOUSE_DEFAUT_X, RX_MOUSE_DEFAUT_Y);
		}
}
void RxCamera::Update()
{
		#if _DEBUG
		if(!sdlWindowID)
			{
				RXLOGGERERROR("[RxCamera][Update] NO INIT");
				return;
			}
		#endif


    if (enable) //ne fait rien si, enable à 0;
    {
        const Uint8 *keystate = SDL_GetKeyboardState (NULL); //relève l'état des touches
        //calcul des nouveaux angles à partir de la position de la souris
		ComputeAngles();
        CheckAngles();

        //calcul du vecteur direction de la camera en fonction des angles.
        dir.x = sin(latitude)*cos(longitude);
        dir.z = sin(latitude)*sin(longitude);
        dir.y = cos(latitude);

        //normalisation en fonction du fps
        float dv = FpsNormalizer()* moveSpeed;

        //calcul du déplacement suivant les touches et la direction de la camera
        if ( keystate[SDL_SCANCODE_UP] )
        {
            pos.x += (dir.x * dv);
            pos.y += (dir.y * dv);
            pos.z += (dir.z * dv);
        }
        if ( keystate[SDL_SCANCODE_DOWN] )
        {
            pos.x -= (dir.x * dv);
            pos.y -= (dir.y * dv);
            pos.z -= (dir.z * dv);
        }
        if ( keystate[SDL_SCANCODE_RIGHT] )
        {
            pos.x -= (dir.z * dv);
            pos.z += (dir.x * dv);
        }
        if ( keystate[SDL_SCANCODE_LEFT] )
        {
            pos.x += (dir.z * dv);
            pos.z -= (dir.x * dv);
        }
    }

	lookAt = glm::lookAt(pos,pos+dir,top);
}
int RxCamera::EnableDisable( int b )
{
        enable = !enable; //on inverse la valeur
        if(b > -1) enable = b;
        //si la camera est activé ne pas oublier de replacer la souris par défaut
        //pour éviter les sauts de caméra
        if(enable)
			SDL_WarpMouseInWindow(sdlWindowID,RX_MOUSE_DEFAUT_X, RX_MOUSE_DEFAUT_Y);

        return enable;
}

glm::mat4& RxCamera::LookAt()
{
	return lookAt;
}
