//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#pragma once

#ifndef _RXCAMERA_H_
#define _RXCAMERA_H_


#ifdef WIN32
#include <GL3/gl3w.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

#include <SDL2/SDL.h>


#include "RxMath3D.h"

#if _DEBUG
	#include "RxLogger.h"
#endif

//definition des directions
#define RX_UP_KEY 0
#define RX_DOWN_KEY 1
#define RX_LEFT_KEY 2
#define RX_RIGHT_KEY 3
#define RX_MOUSE_DEFAUT_X 250
#define RX_MOUSE_DEFAUT_Y 250

//valeur par défaut
#define RX_CAMERA_MOVE_SPEED 1.0f
#define RX_CAMERA_ROTATION_SPEED 0.005f
#define RX_CAMERA_DEFAULT_FPS 0.5f
#define RX_CAMERA_USE 1




class RxCamera
{
    protected:
    unsigned int KEY_VALUES[4];

    unsigned int *elapseTime;			//pointeur vers une variables contenant le temps entre 2 images (en millisecond)

    void CheckAngles();					//oblige les angles à rester dans l'intervalle [0-2PI]
	void ComputeAngles();				//transforme le déplacement horizontal / vertical de la souris en angles

    float FpsNormalizer();				//permet de normaliser le déplacement de la camera en fonction du fps

    public:
    int enable;			//camera libre (1) ou fixe (0)
    glm::vec3 pos;			//Position de la camera
    glm::vec3 dir;			//direction de la camera
	glm::vec3 top;			//direction supérieure de la camera
   
	glm::mat4 lookAt;
	
	float moveSpeed;        //facteur de vitesse de deplacement
    float rotationSpeed;        //facteur de vitesse de rotation
    float normalisedSpeed;       //valeur par default retournée par FpsNormalizer si elapseTime==NULL
    float longitude;    //angle (0-360);
    float latitude;     //angle (0-180);

	bool stepFront;
	bool stepBack;
	bool stepLeft;
	bool stepRight;

	SDL_Window* sdlWindowID;


	//active/désactive l'utilisation de la camera
    int EnableDisable( int c  = -1  ); //0 : desactive , 1 : active, -1 : etat contraire

    //definit la position et direction initiale de la camera
    RxCamera(float p_x, float p_y, float p_z, float v_x, float v_y, float v_z);
	RxCamera(glm::vec3 _pos, glm::vec3 _dir, glm::vec3 _top = glm::vec3(0.0f,1.0f,0.0f));

	//position initiale à 0 et camera 
	RxCamera();
	
	void Init(SDL_Window* w);

    //fonction calculant la nouvelle postion et direction de la caméra
    //en fonction des touches claviers et du mouvement de la sourie
	//déplacement libre FPS
    virtual void Update();    
	virtual glm::mat4& LookAt();

	//setter && getter
	void setPosition(float, float, float);  //Définit la position de la camera aux coordonnées x,y,z
    void setDirection(float, float, float); //Définit la direction de vue de la camera selon le vecteur (x,y,z)
	void setTop(float,float,float);			
	void setDirectionPoint(float,float,float);	//Définit le point cible de la caméra. Fonction altérnative à setDirection
	

	void setPosition(glm::vec3 _pos);
	void setDirection(glm::vec3 _dir);
	void setTop(glm::vec3 _top);
	void setDirectionPoint(glm::vec3 _point);
	
	void setElapseTime(unsigned int *_elapseTime){ elapseTime = _elapseTime; }

};

#endif
