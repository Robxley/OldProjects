//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#include "RxTexManager.h"
#include <sstream>


/*RxTexManager &RxTexManager::Instance()
{
	static RxTexManager instance;
	return instance;
}*/

RxTexManager::RxTexManager()
{
	useMipMap = true;
	defaultTexture = GenTexOfQuads(); //texture par défaut
	//tTextureArray.push_back(defaultTexture);
	int flags=IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF;
	int initted=IMG_Init(flags);
	if( (initted&flags) != flags)
	{
		RXLOGGERERROR("[RxTexManager][RxTexManager] IMG_Init: format JPG, NPG ou TIF non supportés! -- "<<IMG_GetError());
	}
}

RxTexManager::~RxTexManager()
{

	if(tTextureArray.size())
	{
		#ifdef _DEBUG
		  RXLOGGERERROR("[RxTexManager][~RxTexManager] Call Missing to Destroy()");
		#endif
		Destroy();		
	}

}
 void RxTexManager::Clear()
 {
		if(tTextureArray.size()) 
		{
				unsigned int i;
				for(i = 0; i < tTextureArray.size(); i++)
					glDeleteTextures(1,&tTextureArray[i].glID);
				tTextureArray.clear();
		}
 }

 void RxTexManager::Destroy()
 {
	
	Clear();
	glDeleteTextures(1,&defaultTexture.glID);
	IMG_Quit();
	RXLOGGER("[RxTexManager][Destroy] ok");

 }

RxTexture& RxTexManager::LoadTexture(const char *filename)
{


      /** verification de l'existance**/
      if(filename == NULL)
	  {
		  RXLOGGERERROR("[RxTexManager][LoadTex] filename =  NULL");
		  return defaultTexture;
	  }
      //verifie que la texture n'existe pas deja
      RxTexture &prxTexture = getTexture(filename);
      if(prxTexture.glID != defaultTexture.glID) return (prxTexture);  //si elle existe on retourne son ID

      //la texture n'existe pas, on la charge avec SDL
      SDL_Surface * texture;
      texture = IMG_Load (filename); //charge l'image avec la SDL
	 //texture=IMG_Load_RW(SDL_RWFromFile(filename, "rb"), 1);

      //verifie qu'il n'y a pas d'erreur de chargement
      if(texture == NULL)
	  {
		  RXLOGGERERROR("[RxTexManager][LoadTex] Fail IMG_Load("<<filename<<")");
		  return defaultTexture;
	  }
		


      /** --- Création d'un objet de texture Opengl. --**/
      GLuint IDtex = SDL_GL_LoadTexture(texture,useMipMap);

      //Suppression de la surface sdl
      SDL_FreeSurface(texture);

      /***ajout de la texture aux données*****/

      tTextureArray.push_back(RxTexture(IDtex,filename,useMipMap));

	  RXLOGGER("[RxTexManager][LoadTex] Texture : "<<filename);
      //on returne l'ID de la texture
      return tTextureArray.back();
}
RxTexture &RxTexManager::getTexture(const char *filename)
{
    //Pas de nom ou pas d'image, on return 0
    if( (tTextureArray.empty()) || (filename == NULL) ) return defaultTexture;

    unsigned int k;
    //recherche si la texture existe deja à partir de son nom
    std::string test = filename;
	for(k = 0; k < tTextureArray.size() ; k++)
    {
        //si la texture est deja existante on returne son id
        if ( tTextureArray[k].name == test )
                  return tTextureArray[k];

    }


    //on a rien trouvé on renvoit la texture par defaut
    return defaultTexture;

}
RxTexture &RxTexManager::getTexture(GLuint id)
{
    //Pas de nom ou pas d'image, on return 0
    if( (tTextureArray.empty()) || (id == 0) ) return defaultTexture;

    unsigned int k;
    //recherche si la texture existe à partir de son id
	for(k = 0; k < tTextureArray.size() ; k++)
    {
        //si la texture est existante
        if ( tTextureArray[k].glID == id )
                  return tTextureArray[k];

    }

    //on a rien trouvé on renvoit 0
    return defaultTexture;
}

bool RxTexManager::DelTexture(const char *filename)
{
	//Pas de nom ou pas d'image, on return 0
    if( (tTextureArray.empty()) || (filename == NULL) ) return 0;

    unsigned int k;
    //recherche si la texture existe deja à partir de son nom
    for(k = 0; k < tTextureArray.size() ; k++)
    {
        //si on la trouve on la supprime
        if ( tTextureArray[k].name == filename )
		{
				  glDeleteTextures(1,&tTextureArray[k].glID);
			      tTextureArray.erase (tTextureArray.begin()+k);
                  return 1;
		}

    }
	return 0;
}
bool RxTexManager::DelTexture(int glID)
{
	//Pas de nom ou pas d'image, on return 0
    if( tTextureArray.empty() ) return 0;

    unsigned int k;
    //recherche si la texture existe deja à partir de son ID
    for(k = 0; k < tTextureArray.size() ; k++)
    {
        //si on la trouve on la supprime
        if ( tTextureArray[k].glID == glID )
		{
			glDeleteTextures(1,&tTextureArray[k].glID);
			tTextureArray.erase (tTextureArray.begin()+k);
            return 1;
		}

    }
	return 0;
}
bool RxTexManager::DelTexture(RxTexture &texture)
{
	
	//Pas de nom ou pas d'image, on return 0
    if( tTextureArray.empty() ) return 0;

    unsigned int k;
    //recherche si la texture existe deja à partir de son ID
    for(k = 0; k < tTextureArray.size() ; k++)
    {
        //si on la trouve la même addresse on la supprime
        if ( &tTextureArray[k] == &texture )
		{
			      tTextureArray.erase (tTextureArray.begin()+k);
				  glDeleteTextures(1,&tTextureArray[k].glID);
                  return 1;
		}

    }
	return 0;
}
bool RxTexManager::AddTexture(RxTexture &texture)
{
	if (  !(defaultTexture.name == getTexture(texture.name.c_str()).name) ) //le nom existe deja
		return 0;

	tTextureArray.push_back(texture);
	return 1;
}


RxTexture & RxTexManager::getDefaultTexture()
{
	return defaultTexture;
}


RxTexture& RxTexManager::GenTexOfQuads(float r, float g, float b, float a, unsigned int size, std::string name)
{
	    RxTexture &texture = getTexture(name.c_str());	

	    if( texture.glID != defaultTexture.glID )  return texture; //un texture de même nom existe

		std::vector<GLfloat>  colorMap(size*size*4,0);
		
	
		unsigned int i,j;

		for(j = 0; j < size; j++)
		{
			for(i = 0; i < size; i++)
			{
				
				//if( (i < size/2 && j < size/2 ) || (i >= size/2 && j >= size/2 )  )
				if( (i%2 && j%2 ))
				{
					colorMap[i*4 + j*4*size + 0] = r;
					colorMap[i*4 + j*4*size + 1] = g;
					colorMap[i*4 + j*4*size + 2] = b;
					colorMap[i*4 + j*4*size + 3] = a;
				}
				else 
				{
					colorMap[i*4 + j*4*size + 0] = 1.0f;
					colorMap[i*4 + j*4*size + 1] = 1.0f;
					colorMap[i*4 + j*4*size + 2] = 1.0f;
					colorMap[i*4 + j*4*size + 3] = 1.0f;
				}
			}
		}
		GLuint ID = 0;
		glGenTextures(1,&ID);  	//Génère un n° de texture
		if(!ID) return defaultTexture;
		glBindTexture(GL_TEXTURE_2D,ID); 	//Sélectionne ce n°
		glTexImage2D (
		GL_TEXTURE_2D, 	//Type : texture 2D
		0, 	//Mipmap : aucun
		GL_RGBA, 	//Couleurs : 4
		size, 	//Largeur : 2
		size, 	//Hauteur : 2
		0, 	//Largeur du bord : 0
		GL_RGBA, 	//Format : RGBA
		GL_FLOAT, 	//Type des couleurs
		&colorMap[0] 	//Addresse de l'image
		); 	
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,0);
		tTextureArray.push_back(RxTexture(ID,name));

		return tTextureArray.back();
}
RxTexture&  RxTexManager::CreateArrayTexture( std::vector<std::string> &filenames, const char *name)
{
	std::ostringstream sname;
	
	bool error = 0;
	GLuint idTex = 0;
	if(name != NULL)
		sname<<name;
	else
		sname<<filenames.size()<<"arrayImage"<<tTextureArray.size();

	//chargement des images dans une surface
	std::vector<SDL_Surface*> surface_list;
	surface_list.resize(filenames.size(),NULL);
	unsigned int k;
	for(k = 0; k < filenames.size(); k++)
	{
		surface_list[k] = IMG_Load(filenames[k].c_str()); //charge l'ensemble des images
	}
	
	//verifie que les images sont toutes de même format
	for(k = 1; k < filenames.size(); k++)
	{
		if( (surface_list[k] == NULL) ||
			(surface_list[0]->format->BytesPerPixel != surface_list[k]->format->BytesPerPixel) ||
			(surface_list[0]->h != surface_list[k]->h) ||
			(surface_list[0]->w != surface_list[k]->w) 
		  )
		{
			error = 1;
			RXLOGGERERROR("[RxTexManager][CreateArrayTex] Unmatched image dimensions : "<<sname.str());
			break;
		}
	} 

	int r = filenames.size(); // et autant de ligne que d'image

	//pas d'erreur les images sont bien de meme taille
	//concaténation
	SDL_Surface *multiImgSurf = NULL;
	if( !error ) 
	{
		unsigned int width, height;
		//détermination des dimensions
		width = surface_list[0]->w;
		height = surface_list[0]->h*r;
		//creation d'une surface vierge
		
		Uint32 rmask, gmask, bmask, amask;
	   /* SDL interprets each pixel as a 32-bit number, so our masks must depend
		   on the endianness (byte order) of the machine */
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
		#else
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0x00000000;
		#endif

		//allocation de la surface nécessaire à la concaténation des images
		multiImgSurf = SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,surface_list[0]->format->BitsPerPixel,rmask,gmask,bmask,amask);
		
		if( multiImgSurf != NULL)
		{
			unsigned int i,j,k,w,h;
			w = surface_list[0]->w;
			h = surface_list[0]->h;
			SDL_Rect position;

			//pour chaque image on les copies unes après les autres sur la même surface
			for(k = 0; k < surface_list.size(); k ++)
			{
				//position de l'image dans l'image global
				i = k;
				j = k - i;
				position.y = i*h;
				position.x = j*w;
				//recopie de l'image à la bonne position
				SDL_BlitSurface(surface_list[k], NULL, multiImgSurf, &position);
			}
				//ajoute l'image au contexte opengl
				/** --- Création d'un objet de texture Opengl. --**/
			  idTex = SDL_GL_LoadTextureArray(multiImgSurf,filenames.size(),useMipMap);
			  
			  /***ajout de la texture aux données*****/
			  if(idTex)	
				tTextureArray.push_back(RxTexture(idTex, sname.str().c_str(),useMipMap,RX_ARRAY_TEX,filenames.size(),1,filenames.size() ));

		}

	}
	
	//libération de l'espace mémoire
	for(k = 0; k < filenames.size(); k++)
	{
		SDL_FreeSurface(surface_list[k]); //charge l'ensemble des images
	}
	SDL_FreeSurface(multiImgSurf);

    if(!idTex) return defaultTexture;

	RXLOGGER("[RxTexManager][CreateArrayTex] ArrayTexture : "<<name);
	return tTextureArray.back();
}
RxTexture& RxTexManager::CreateMultiTexture( std::vector<std::string> &filenames, int r, int c, const char* name)
{
	std::ostringstream sname;
	
	bool error = 0;
	GLuint idTex = 0;
	if(name != NULL)
		sname<<name;
	else
		sname<<filenames.size()<<"multiImage"<<tTextureArray.size();

	//chargement des images
	std::vector<SDL_Surface*> surface_list;
	surface_list.resize(filenames.size(),NULL);
	unsigned int k;
	for(k = 0; k < filenames.size(); k++)
	{
		surface_list[k] = IMG_Load(filenames[k].c_str()); //charge l'ensemble des images
	}
	
	//verifie que les images sont toutes de même format
	for(k = 1; k < filenames.size(); k++)
	{
		if( (surface_list[k] == NULL) ||
			(surface_list[0]->format->BytesPerPixel != surface_list[k]->format->BytesPerPixel) ||
			(surface_list[0]->h != surface_list[k]->h) ||
			(surface_list[0]->w != surface_list[k]->w) 
		  )
		{
			error = 1;
			RXLOGGERERROR("[RxTexManager][CreateMultiTex] Unmatched image dimensions : "<<sname.str());
			break;
		}
	} 

	//pas d'erreur les images sont bien de meme taille
	//concaténation
	SDL_Surface *multiImgSurf = NULL;
	if( !error ) 
	{
		unsigned int width, height;
		//détermination des dimensions
		width = surface_list[0]->w*c;
		height = surface_list[0]->h*r;
		//creation d'une surface vierge
		
		Uint32 rmask, gmask, bmask, amask;
	   /* SDL interprets each pixel as a 32-bit number, so our masks must depend
		   on the endianness (byte order) of the machine */
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
		#else
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0x00000000;
		#endif

		multiImgSurf = SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,surface_list[0]->format->BitsPerPixel,rmask,gmask,bmask,amask);
		
		if( multiImgSurf != NULL)
		{
			unsigned int i,j,k,w,h;
			w = surface_list[0]->w;
			h = surface_list[0]->h;
			SDL_Rect position;

			for(k = 0; k < surface_list.size(); k ++)
			{
				//position de l'image dans l'image global
				i = k/c;
				j = k - i * c;
				position.y = i*h;
				position.x = j*w;
				//recopie de l'image à la bonne position
				SDL_BlitSurface(surface_list[k], NULL, multiImgSurf, &position);
			}
				//ajoute l'image au contexte opengl
				/** --- Création d'un objet de texture Opengl. --**/
			  idTex = SDL_GL_LoadTexture(multiImgSurf,useMipMap);
			   
			  /***ajout de la texture aux données*****/
			 if(idTex)
			   tTextureArray.push_back(RxTexture(idTex, sname.str().c_str(),useMipMap,RX_MULTI_TEX,r,c,filenames.size() ));

		}

	}
	

	//libération de l'espace mémoire
	for(k = 0; k < filenames.size(); k++)
	{
		SDL_FreeSurface(surface_list[k]); //charge l'ensemble des images
	}
	SDL_FreeSurface(multiImgSurf);
    if(!idTex) return defaultTexture;
	
	RXLOGGER("[RxTexManager][CreateMultiTex] MultiTexture : "<<name);
	return tTextureArray.back();
}
GLuint RxTexManager::SDL_GL_LoadTextureArray(SDL_Surface *surface,unsigned int nb_tex, bool mipmap)
{
    GLuint texture;
    int w, h;
    SDL_Surface *image;
    SDL_Rect area;


    /* Use the surface width and height expanded to powers of 2 */
	//  w = power_of_two(surface->w);
	//  h = power_of_two(surface->h);

	w = surface->w;
    h = surface->h;

    image = SDL_CreateRGBSurface(
            SDL_SWSURFACE,
            w, h,
            32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
            0x000000FF,
            0x0000FF00,
            0x00FF0000,
            0xFF000000
#else
            0xFF000000,
            0x00FF0000,
            0x0000FF00,
            0x000000FF
#endif
               );
    if ( image == NULL ) {
        return 0;
    }


    /* Copy the surface into the GL texture image */
    area.x = 0;
    area.y = 0;
    area.w = surface->w;
    area.h = surface->h;
    SDL_BlitSurface(surface, &area, image, &area);


	
    /* Create an OpenGL texture for the image */

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT) ;
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT) ;


	glTexImage3D(GL_TEXTURE_2D_ARRAY ,0,GL_RGBA,w, h/nb_tex, nb_tex,0,GL_RGBA,GL_UNSIGNED_BYTE,image->pixels);
	if(mipmap)
	{
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	}

    SDL_FreeSurface(image);

    return texture;
}
GLuint RxTexManager::SDL_GL_LoadTexture(SDL_Surface *surface, bool mipmap)
{
    GLuint texture;
    int w, h;
    SDL_Surface *image;
    SDL_Rect area;


    /* Use the surface width and height expanded to powers of 2 */
	//  w = power_of_two(surface->w);
	//  h = power_of_two(surface->h);

	w = surface->w;
    h = surface->h;

    image = SDL_CreateRGBSurface(
            SDL_SWSURFACE,
            w, h,
            32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
            0x000000FF,
            0x0000FF00,
            0x00FF0000,
            0xFF000000
#else
            0xFF000000,
            0x00FF0000,
            0x0000FF00,
            0x000000FF
#endif
               );
    if ( image == NULL ) {
        return 0;
    }


    /* Copy the surface into the GL texture image */
    area.x = 0;
    area.y = 0;
    area.w = surface->w;
    area.h = surface->h;
    SDL_BlitSurface(surface, &area, image, &area);


	
    //création de la texture opengl et remplissage
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if(mipmap) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	
	glTexImage2D(GL_TEXTURE_2D,
	 0,
	 GL_RGBA,
	 w, h,
	 0,
	 GL_RGBA,
	 GL_UNSIGNED_BYTE,
	 image->pixels);
	
	if(mipmap) glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(image); /* No longer needed */

    return texture;
}

void RxTexManager::setDefaultTexture(RxTexture &id){ defaultTexture = id;};
std::vector<RxTexture> &RxTexManager::getTextureTab(){ return tTextureArray;}