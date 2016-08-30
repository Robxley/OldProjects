//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

//==========================================================
// TURORIAL developpez.com moteur 3D
//
// Définition de la classe RxFile
//
//==========================================================

#ifndef _RXFILETOOLS_H_
#define _RXFILETOOLS_H_

//==========================================================
// En-têtes
//==========================================================

#include <string>



    //==========================================================
    // Classe facilitant la manipulation des fichiers
    //==========================================================
    class RxFile
    {
    public :

        //----------------------------------------------------------
        // Constructeur à partir d'un std::string
        //----------------------------------------------------------
        RxFile(const std::string& Name = "unknown");
        

        //----------------------------------------------------------
        // Constructeur à partir d'un const char*
        //----------------------------------------------------------
        RxFile(const char* Name);

		//----------------------------------------------------------
        // Constructeur à partir d'un std::string
        //----------------------------------------------------------
        void set(const std::string& Name = "unknown");

        //----------------------------------------------------------
        // Indique si le fichier existe ou non
        //----------------------------------------------------------
        bool Exists() const;

        //----------------------------------------------------------
        // Renvoie le nom du fichier avec son chemin complet 
		// (Dir+filename) ex : data/maison/porte.jpg
        //----------------------------------------------------------
        const std::string& Fullname() const;

        //----------------------------------------------------------
        // Renvoie le nom du fichier sans son chemin
		// ex : porte.jpg
        //----------------------------------------------------------
        std::string Filename() const;

        //----------------------------------------------------------
        // Renvoie le nom du fichier sans extension ni chemin
		// ex : porte
        //----------------------------------------------------------
        std::string ShortFilename() const;

        //----------------------------------------------------------
        // Renvoie l'extension du fichier
		// ex : jpg
        //----------------------------------------------------------
        std::string Extension() const;

        //----------------------------------------------------------
        // Renvoie l'extension du fichier
		// ex : data/maison
        //----------------------------------------------------------
        std::string Dir() const;
    private :

        //----------------------------------------------------------
        // Données membres
        //----------------------------------------------------------
		std::string fullName; // Chemin complet du fichier
    };



#endif // FILE_H
