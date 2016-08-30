//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#


#pragma once



#ifndef _RXEXTENSION_H_
#define _RXEXTENSION_H_

#include <GL/glew.h>
#include <iostream>
#include "RxLogger.h"
/* verifie si l'extension 'nom' est supportee et l'initialisee */
bool ExtensionSupportee(const char *nom);

/* initialise les extensions en verifiant leur support */
bool InitExtensions();

#endif