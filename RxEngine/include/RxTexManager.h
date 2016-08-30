//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#pragma once
#ifndef _RXTEXMANAGER_H_
#define _RXTEXMANAGER_H_


#ifdef WIN32
#include <GL3/gl3w.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif


#include <sdl2/sdl.h>
#include <sdl2/sdl_image.h>
#include <string>
#include <iostream>
#include <vector>


#include "RxConst.h"
#include "RxLogger.h"
#include "RxSingleton.h"


#define SIZE_NAME_MAX 256




enum RX_TEXTURE_TYPE
{
	RX_SIMPLE_TEX,  //image simple (par defaut)
	RX_MULTI_TEX,   //fusion de plusieurs image en une image multiple
	RX_ARRAY_TEX,   //tableau de texture
};


typedef struct RxTexture
{
    GLuint glID;
    std::string name;
	bool useMipMap;

	//paramètre multi/array image
	// type = RX_SIMPLE_TEX : image simple (par defaut)
	// type = RX_MULTY_TEX  : fusion de plusieurs image en une image
	// type = RX_ARRAY_TEX  : tableau de texture
	int type; 
	unsigned int row, col; //nombre de ligne / colonne de notre image multi
	unsigned int n; //nombre de texture dans l'image/tableau.
   
	RxTexture(GLuint _glID = 0, std::string _name = "", bool mipmap = 0, int _type = RX_SIMPLE_TEX,unsigned int r = 1,unsigned int c = 1, unsigned int _n_multi=1)
    {
        glID = _glID;
        name = _name;
		useMipMap = mipmap;
		type = _type;
		row = r;
		col = c;
		n = _n_multi;
    }
    RxTexture(const RxTexture &t)
    {
        this->glID = t.glID;
        this->name = t.name;
		this->useMipMap = t.useMipMap;
		this->type = t.type;
		this->row = t.col;
		this->col = t.col;
		this->n = t.n;
    }

    RxTexture& operator=(const RxTexture &t)
    {
        this->glID = t.glID;
        this->name = t.name;
		this->useMipMap = t.useMipMap;
		this->type = t.type;
		this->row = t.col;
		this->col = t.col;
		this->n = t.n;
		return *this;
    }
	
	//opérateur
	operator GLuint() { return this->glID; }
	operator const GLuint() const { return this->glID; }

	operator GLint() { return this->glID; }
	operator const GLint() const { return this->glID; }

} RxTexture;

class RxTexManager : public RxSingleton<RxTexManager>
{
	friend RxSingleton<RxTexManager>;
    private:
        std::vector<RxTexture> tTextureArray;     //tableau de texture
		bool useMipMap;
		RxTexture defaultTexture;
		
		RxTexManager();
        ~RxTexManager(); 

		GLuint SDL_GL_LoadTextureArray(SDL_Surface *surface,unsigned int nb_tex, bool mipmap = true);
		GLuint SDL_GL_LoadTexture(SDL_Surface *surface, bool mipmap = true);

    public:

		
	   void setMipMap(bool mipmap){ useMipMap = mipmap;}

        /***********************************************************/
        /*                                                         */
        /* Recherche si l'image existe déjà à partir de son nom.   */
        /* Si l'image n'existe pas, chargement de celle-ci dans    */
        /* un context opengl et return son ID d'itentification     */
        /* Si l'image existe retourne son ID.                      */
        /***********************************************************/
       RxTexture& LoadTexture(const char *filename);
		

	   /*****************************************************************/
	   /* Concaténation d'un ensemble d'image de même taille         	*/
	   /* en une même image												*/
	   /* filenames : liste des images									*/
	   /* l, c : format de concaténation l : lignes, c :colone			*/
	   /* name : nom donné à la nouvelle image créée					*/
	   /*****************************************************************/
	   RxTexture& CreateMultiTexture( std::vector<std::string> &filenames, int r = 1, int c = 1, const char* name = NULL);
       RxTexture& CreateArrayTexture( std::vector<std::string> &filenames, const char *name = NULL);
	   //Trouve l'ID de la texture correspondant à filename. defaultTex si rien trouvé.
       RxTexture& getTexture(const char *filename);



	   //supprime l'ensemble des textures chargées (defautTex n'est pas supprimée)
	   void Clear();

	   //Detruit le singleton RxTexManager
	   void Destroy();

       // supprime la texture à partir de son nom ou son ID
       // si suppression réussie retourne 1
       // si non 0 si ID ou nom non trouvé
       bool DelTexture(const char *filename); 
       bool DelTexture(int glID); 
	   bool DelTexture(RxTexture & texture);


	   //rajoute une texture à l'ensemble des textures.
	   //attention lors de l'appel du destructeur son context GL sera détruit
	   //si une texture du même nom existe deja revoie 0 et la texture n'est pas copié
	   //si tout se passe bien renvoie 1
	   bool AddTexture(RxTexture &texture);

	   std::vector<RxTexture> &getTextureTab();
	   RxTexture &getDefaultTexture();
	   void setDefaultTexture(RxTexture &id);
	   RxTexture& GenTexOfQuads(float r = 1.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f, unsigned int size = 16, std::string name = "defaultTexture");
	   RxTexture& getTexture(GLuint id); //return defaultTex si n'existe pas
};

#endif
