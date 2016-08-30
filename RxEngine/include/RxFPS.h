//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#pragma once
#ifndef _RXFPS_H_
#define _RXFPS_H_

#include <SDL2/SDL.h>

class RxFPS
{

    public:
        RxFPS();
        //retourne le nombre moyen d'image par second
        // (attend 1sec et regarde le nombre d'image
        unsigned int getFPS();


		//Retourne le temps écoulé en millisecond depuis le 
		//dernier appelle de getFPS()
		unsigned int getElapseTime();

        //calcul du fps ( à placer dans la boucle de l'application)
		void Compute();
	private:
        unsigned int elapseTime; //temps entre deux images
        unsigned int fps; //fps moyen sur 1 sec


		//variables
		unsigned int sumTime, lastTime, count;
};

#endif
