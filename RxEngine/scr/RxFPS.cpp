//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#include "RxFPS.h"



RxFPS::RxFPS()
{
 //initialise toutes les variables
	elapseTime = 0;
	fps = 0;
	sumTime = 0;
	lastTime = SDL_GetTicks();
	count = 0;
}
unsigned int RxFPS::getFPS()
{
    return fps;
}
unsigned int RxFPS::getElapseTime()
{
    return elapseTime;
}

void RxFPS::Compute()
{
	
    unsigned int t = 0;
    t = SDL_GetTicks(); 

	elapseTime =(t-lastTime);
	lastTime = t;


	//fps moyen
	sumTime+=elapseTime;
    if( sumTime > 1000)
    {
        fps = count;
        count = 0;
        sumTime = 0;
    }
    else
        count++;

}
