//#============================================================#
//#        Copyright � 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#pragma once
#ifndef _RXWINDOW_H_
#define _RXWINDOW_H_



#ifdef WIN32
#include <GL3/gl3w.h>
#else
#define GL3_PROTOTYPES 1
#include <GL3/gl3.h>
#endif

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <vector>


#include "RxFont.h"
#include "RxVBO.h"
#include "RxMath3D.h"


#define RX_BTN_NONE 0
#define RX_BTN_LEFT 1
#define RX_BTN_MIDDLE 2
#define RX_BTN_RIGHT 3

//textlist
//taille par d�faut des boutons de d�filement de text
#define RX_BTN_WIDTH 20
#define RX_BTN_HEIGHT 20


#define RX_VERT_SHADER_ITEM2D "shaders/item2d.vert"
#define RX_FRAG_SHADER_ITEM2D "shaders/item2d.frag"




enum RX_ITEMS_2D
{
	RX_ITEM,
	RX_BUTTON,
	RX_TEXTBOX,
	RX_WINDOW,
};




/*************************************************************************/
/*** CLASSE RxItem2D *******************************************************/
class RxItem2D
{
   protected:
        /******** Donn�es g�om�triques ***********************************************/
        //donn�es g�ometries de l'objet 2D ( position, taille ).
		//position r�elle � l'�cran (non local � un objet)
        int pos_x, pos_y, width, height;

		int type;
        /******** Donn�es d'�tat ******************************************************/
        //attribues et caract�ristiques de l'objet 2D.
        // visible : determine si l'objet est affich� � l'�cran ou non.
        // movable : s'il est possible de d�placer l'objet.
        // outside : si l'objet peut sortir du cadre de l'objet m�re.
        int visible, movable, outside;

        //permet d'identifier l'objet consern� par un num�ro
        int ID;

        //nombre d'item creer sur l'ensemble des classes
        static unsigned int item_counter;

         /******* Donn�es graphiques **************************************************/

        float R,G,B,A;      //couleur de notre objet ( par d�faut : 1.0f, 1.0f ,1.0f)
        int idTex;        //ID de la texture opengl

        RxFont *font;       //passe en pointeur la font
        int text_x, text_y; //position du texte sur l'objet
        float R_text, G_text, B_text, A_text;//couleur du texte sur l'objet
        std::string text;        //le texte de l'item
		RxShader *ptrShader;//si l'on shoutaite utiliser un autre shader que celui par d�faut



		
		
		static RxVBO* pvbo;//affichant un carr� de dimension 1 � l'�cran 2d (base commune � tous les item2d)
		static RxShader *pshader;//shader utilis�e pour l'afficher les objets
		static void CreateInstanceVBOShader(); //creation du vbo/shader
		/******** Donn�es d'interaction **************************************************************/
        int lastMouse_x, lastMouse_y, lastBouton;


        static int focus; //afin de savoir quel objet � le focus
        //bouton interagissant avec l'objet
        // (1: bouton gauche, 2: bouton du milieu, 3 : bouton droite)
        // par d�faut le bouton gauche
        int bouton_enable;

        //determine si la souris est sur l'item
        virtual int MouseOnItem(int mx, int my);

        //dans le cas ou outside est � 0 verifie que l'item est bien dans l'objet m�re
        //si non on replace l'objet sur les bords de l'objet m�re
        virtual void ItemStayWithFather();

		/******* interaction entre objets ****************************************************/
		//tableau de sauvegarde de toutes les instances des item2d;
		static std::vector<RxItem2D*> &BigFather();
        //pointeurs sur l'objet parent
        // si father = NULL. Objet father = la fenetre/viewport.
        // si non tout autre item2d
        RxItem2D *father;

		//fonction de CallBack appel� automatiquement apr�s
		//l'�xecution des m�thodes de type CallBack*()
		//n : variable pass� � fctCallBack et correspondant � la
		//valeur renvoi� par les m�thodes CallBack*()
		//item : pointeur sur l'objet.
		void (*fctCallBack)(int n, RxItem2D* item);

    public:

        //constructeur/destructeur
        RxItem2D();
		RxItem2D(RxItem2D & item); //copy par rapport un un item existant
        virtual ~RxItem2D();

        //Fonction d'interaction avec l'objet 2D (
        //retourne -1 si la sourie se trouve en dehors de l'objet
        //si non 0 si aucun bouton press�
        //si non 1 si bouton gauche press�
        //si non 2 si bouton milieu press�
        //si non 3 si bouton droit press�
        //Et executre la fontion fctUsedByCallBack
        //m�thode par analyse de la position des objets et coordonn�es de la sourie
        //Ne fonction que dans le cas d'une projection othonorm�e de l'item, c'est � dire que les
        //coordonn�es de l'item correspond aux coordonn�es pixel de l'�cran
        virtual int CallBackCoord();

  
		void setCallBackFct( void(*fct)(int,RxItem2D*));

        virtual void Draw(); //dessine notre objet dans le contexte opengl

		//cr�e une copie et retourne le pointeur asscoi�
		//attention la copie cr��e n'est pas automatique supprim� lors de la destruction de l'instance copi�e
		virtual RxItem2D* Copy();

        //modifieur (no comments)
        virtual void setPosition(int _x, int _y);
        virtual void setSize(int _w, int _h);
        virtual int setVisible(int v);
        virtual int setMovable(int m);
        virtual int setOutside(int o);
        virtual void setFont(RxFont *_f);
        virtual unsigned int setTexture(unsigned int id);
        virtual void setColor(float _r, float _g, float _b, float _a = 1.0f);

		//interconnection entre les diff�rents objets.
		virtual void setFather( RxItem2D * _father = NULL);

		//definition de la position dans le rep�re local du p�re
		//si p�re �gale null, la position local et global sont confondues
		virtual void setLocalFatherPosition(int _x, int _y);
		virtual void setLocalFatherPosition(glm::ivec2 pos);




        //change le texte
        virtual void setText(const char * t);
        virtual void setPosText(int _x, int _y);
		virtual void setColorText(float _r, float _g, float _b, float _a=1.0f);

		//dependance Math3d
#ifdef _RXMATH3D_H_
		virtual void setPosition(glm::ivec2 pos);
		virtual void setSize(glm::ivec2 size);
		virtual void setColor(glm::vec4 color);
		virtual void setPosText(glm::ivec2 postext);
		virtual void setColorText(glm::vec4 color);
#endif
		


        //accesseurs (no comments)
        virtual int getPosX();
        virtual int getPosY();
        virtual int getWidth();
        virtual int getHeight();
        virtual int getVisible();
        virtual int getMovable();
        virtual int getOutside();
        virtual const char *getText();
        virtual int getID();

		//en commentaire, m�me travail que le constructeur g�n�r� automatiquement
		virtual RxItem2D& operator=(RxItem2D& item);


};
/*************************************************************************/
/*** CLASSE RxButton *******************************************************/
class RxButton:public RxItem2D
{
	private:
        //un bouton est un ITEM2D sur lequel il est possible de cliquer
        //le clic du bouton introduisant souvent un chagement de celui-ci
        //tels qu'un changement de couleur, texture, ...

        int idTexMouseOut; // texture lorsque la souris n'est pas sur le bouton
        int idTexMouseOn;  //ID de la texture lorsque la souris se trouve sur le bouton
        int idTexMouseClick;   //ID de la texture lorsque la souris est press� sur le bouton.

        std::string textMouseOut; // idem mais pour le texte du bouton
        std::string textMouseOn;  //
        std::string textMouseClick;   //

        float R_MouseOut,G_MouseOut,B_MouseOut, A_MouseOut; // idem mais pour la couleur du bouton
        float R_MouseOn, G_MouseOn, B_MouseOn, A_MouseOn;  //
        float R_MouseClick, G_MouseClick, B_MouseClick, A_MouseClick;     //
		


    public:
        RxButton();
        ~RxButton();

        //attribut les textures pr les diff�rents �tats du bouton
        //si idtexout est seulement pass� en attribut, les autres �tats poss�deront
        //la m�me texture que idTexOutMouse;
        //param 1 : ID de la texture si la souris est en dehors du bouton
        //param 2 : ID de la texture si on passe la sourie sur le bouton
		//param 3 : ID de la texture si on clique sur le bouton
        void setIDsTex( int idtexmouseout, int idtexmouseon = 0, int idtexmouseclick = 0);

        //fonction callback du bouton, reprend la fonction callback de ITEM2D et
        //attribut les graphiques actifs du bouton on fonction du l'etat de celui-ci;
        int CallBackCoord();

		//cr�e une copie et retourne le pointeur
		//attention la copie cr��e n'est pas automatique supprim� lors de la destruction de l'instance copi�e
		RxItem2D * Copy();

        //attribut les couleurs pr les diff�rents �tats du bouton
        //si la premiere couleur est seulement pass� en param�tre, les autres �tats poss�deront
        //la m�me couleur;
        //Ordre des param�tres R,G,B d�finit de la m�me facon que pour les textures
        void setColors
        (
            float r_out        , float g_out        , float b_out, float a_out = 1.0f,
			float r_on=-1      , float g_on=-1      , float b_on=-1, float a_on=1.0f,
            float r_click=-1  , float g_click=-1  , float b_click=-1, float a_click=1.0f
          
        );

        //Permet de d�finir le texte � afficher en fonction de l'�tat de la souris
        //si seul le texte tOut est pass� le bouton affichera se meme texte pour tout les �tats
        void setTexts(const char* tOut, const char* tOn = NULL, const char* tClick = NULL);

#ifdef _RXMATH3D_H_
		 void setColors
        (
            glm::vec4 rgb_out,
            glm::vec4 rgb_on = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f),
            glm::vec4 rgb_click = glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f)
        );
		void setColors
        (
            glm::vec3 rgb_out,
            glm::vec3 rgb_on = glm::vec3(-1.0f),
            glm::vec3 rgb_click = glm::vec3(-1.0f)
        );
#endif

};





/*****************************************************************************/
/*** CLASSE RxTextBox ********************************************************/
class RxTextBox:public RxItem2D
{
    private:


        //dans la zone de texte.
        std::string original_text; //valeur du text original
        std::string format_text; //texte reformat� pour notre textbox
        int nb_lines; //nombre de lignes dans le texte format�
        int nb_lines_to_shows; //nombre de ligne que l'on peut afficher simultan�ment dans notre text box
        int line_start; //num�ro de la ligne o� on commence a afficher notre text

        int border;

		//partitionnement du texte en ligne
		std::vector<std::string> textbyline;


        /*variable jouant sur la vitesse de defilement du texte
        lorque l'on appuie sur les bouton de bas/haut**/
        unsigned int *fps;  //pointeur sur le fps
        float speed_text; //vitesse de d�filement
		


        //m�thodes
		int getRealWidth();
		void ItemStayWithFather();
        //copie les lignes � afficher en fonction de line_start et nb_lines_to_shows;
        void CpyVisibleLinesToText();
       
		//formatage du texte pour qu'il s'affiche correctement dans la textbox
		void KeepTextInBox();

		void CreateTextByline();

		void MoveScroll();
		void MoveTextWithScoll();
    public:
		RxButton BUp, BDown,BScroll,BOnScroll; //bouton permettant de faire d�filer le texte


        RxTextBox();
		RxTextBox(RxTextBox & item);
        ~RxTextBox();
      
        void setPosition(int _x, int _y);
		void setFather( RxItem2D * _father = NULL);
        void setFont(RxFont *_f);
        void setText(const char *_t);
        void setBorder(int b);
		void setSpeedScroll(float speed);
		//d�finit la taille du champ de texte seulement
		void setSize(int _w, int _h);


        int CallBackCoord();
        void Draw();

		//cr�e une copie et retourne le pointeur
		//attention la copie cr��e n'est pas automatique supprim� lors de la destruction de l'instance copi�e
		RxItem2D * Copy();

        /***/
        int getLineStart() { return line_start; }

        /*variable jouant sur la vitesse de defilement du texte
        lorque l'on appuie sur les bouton de bas/haut**/
        void setFpsPointer(unsigned int *_fps);  //pointeur sur le fps
        void setSpeed(float sv);; //vitesse de d�filement


		//en commentaire, m�me travail que le constructeur g�n�r� automatiquement
		virtual RxTextBox& operator=(RxTextBox& item);
};




/***************************************************************************/
/*** CLASSE RxWindow *******************************************************/
class RxWindow:public RxItem2D
{
    private:
        int nb_B; //nombre de bouton
		std::vector<RxItem2D*> itemlist;



    public:
		
        RxWindow();
		~RxWindow();
		
		//cr�ation et ajout d'une copie de item (item, button, textbox, windows);
		RxItem2D* AddItem(RxItem2D *item);
		
		//fonction d'ajout d'objet vierge
		RxItem2D* AddLabel();
		RxItem2D* AddButton();
		RxItem2D* AddTextbox();

		//suppression des objets de la fenetre
		void Clear();

		//cr�e une copie et retourne le pointeur
		//attention la copie cr��e n'est pas automatique supprim� lors de la destruction de l'instance
		RxItem2D * Copy();

		//position de la fenetre � l'�cran
		void setPosition(int _x, int _y);
     
		//si n = -1, font appliqu� � l'ensemble des
		//objet appartenant � la fenetre.
		//si n !=-1 font appliqu� uniquement � l'objet � la position n dans itemlist
        void setFont(RxFont *_f , int n = -1);
		
	
		//n�cessite une projection orthonorm�e pour fonctionn�e
		//(indices des pixels correspondants aux positions des objets
		//avec une origine opengl en bas � gauche)
        int CallBackCoord();


        void Draw();
        


		//liste des diff�rents objets de la fenetre
		std::vector<RxItem2D*> &getItemlist();



};
#endif
