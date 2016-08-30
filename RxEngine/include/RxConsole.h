//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#pragma once

#ifndef _RXCONSOLE_H_
#define _RXCONSOLE_H_

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class RxConsole
{
private:
	vector<string> inputcmds; //commandes entrées par l'utilisateur
	vector<string> listcmds;  //liste des commandes utilisables / reconnues
	void execCmd(string cmd); //execution d'une commande
public:
	RxConsole();
	~RxConsole();
	
	void AddInputCmd(string cmd); //
	void NextCmd();
	void PreviousCmd();
	
	string *getInputcmds();
	string *getListcmds();
};
#endif