//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#include "RxWindow.h"



/*************************************************************************/
/*** CLASSE RXITEM2D *******************************************************/

unsigned int RxItem2D::item_counter = 0;
int RxItem2D::focus = -1; //par défaut personne n'a le focus
RxVBO *RxItem2D::pvbo = NULL;
RxShader *RxItem2D::pshader = NULL;


RxItem2D::RxItem2D()
{
	    ptrShader = NULL;
		BigFather().push_back(this);
		type = RX_ITEM;
        pos_x = pos_y = 0; width = height = 20;
        visible = movable = outside = true;
        father = NULL;
        ID = item_counter;
        item_counter++;
        R = G = B = A = 1.0f;  //couleur par defaut blanche
        idTex = 0;
        font = NULL;
        bouton_enable = RX_BTN_LEFT;//bouton gauche actif par défaut
        text_x = text_y = 0;
        R_text = G_text =  B_text = A_text = 1.0f;
		fctCallBack = NULL;
        focus = false;

		if(pvbo == 0 || pshader == 0) //le vbo n'est pas encore initialisé
			CreateInstanceVBOShader();
		
}

RxItem2D::~RxItem2D()
{

   //indique au bigFather que l'objet n'est plus valide
   BigFather()[ID] = NULL;

}

//modifieurs, no comment
void RxItem2D::setPosition(int _x, int _y){ pos_x = _x; pos_y = _y; }
void RxItem2D::setLocalFatherPosition(int _x, int _y)
{
	if(father)
	{
		_x += father->getPosX();
		_y += father->getPosY();
		setPosition(_x,_y);
	}
	else
	{
		setPosition(_x,_y);
	}
	ItemStayWithFather();
}
void RxItem2D::setSize(int _w, int _h){ width = _w; height = _h;}
int RxItem2D::setVisible(int v){ return (visible = v); }
int RxItem2D::setMovable(int m){ return (movable = m); }
int RxItem2D::setOutside(int o){ return (outside = o); }
void RxItem2D::setFather( RxItem2D * _father){ father = _father;}
unsigned int RxItem2D::setTexture(unsigned int id){ return (idTex = id); }
void RxItem2D::setColor(float _r, float _g, float _b, float _a){R = _r; G =_g; B = _b; A =_a;}
void RxItem2D::setFont(RxFont *_f){font = _f;}
void RxItem2D::setPosText(int _x, int _y){ text_x = _x; text_y = _y;}
void RxItem2D::setColorText(float _r, float _g, float _b, float _a)
{R_text = _r; G_text = _g; B_text = _b; A_text = _a;}
void RxItem2D::setText(const char * t)
{
    text.clear();
	text = t;
}

//dependance Math3d
#ifdef _RXMATH3D_H_
void RxItem2D::setPosition(glm::ivec2 pos){setPosition(pos.x,pos.y);}
void RxItem2D::setLocalFatherPosition(glm::ivec2 pos){setLocalFatherPosition(pos.x,pos.y);}
void RxItem2D::setSize(glm::ivec2 size){setSize(size.x,size.y);}
void RxItem2D::setColor(glm::vec4 color){setColor(color.r,color.g,color.b,color.a);}
void RxItem2D::setPosText(glm::ivec2 postext){setPosText(postext.x,postext.y);}
void RxItem2D::setColorText(glm::vec4 color){setColorText(color.r,color.g,color.b,color.a);}
#endif

//accesseurs, no comment
int RxItem2D::getPosX()    { return pos_x;   }
int RxItem2D::getPosY()    { return pos_y;   }
int RxItem2D::getWidth()   { return width;   }
int RxItem2D::getHeight()  { return height;  }
int RxItem2D::getVisible(){ return visible; }
int RxItem2D::getMovable(){ return movable; }
int RxItem2D::getOutside(){ return outside; }
const char *RxItem2D::getText()  { return text.data();    }
int RxItem2D::getID()      { return ID;    }


std::vector<RxItem2D*> & RxItem2D::BigFather()
{
	static std::vector<RxItem2D*> bigFather;

	return bigFather;
}
RxItem2D::RxItem2D(RxItem2D & item)
{
	(*this) = item;

	BigFather().push_back(this);
	type = RX_ITEM;
    ID = item_counter;
    item_counter++;
	fctCallBack = NULL;
    focus = false;
}
RxItem2D& RxItem2D::operator=(RxItem2D& item)
{
	this->setSize(item.width,item.height);
	this->setPosition(item.pos_x,item.pos_y);
	this->setColor(item.R,item.G,item.B);
	this->setColorText(item.R_text,item.G_text,item.B_text);
	this->setFont(item.font);
	//this->setTexture(item.ID);
	this->setTexture(item.idTex);
	this->setMovable( item.movable);
	this->setOutside( item.outside);

	this->setText(item.text.c_str());
	this->setPosText(item.text_x,item.text_y);
	this->setFather(item.father);
	this->ptrShader = item.ptrShader;


	return (*this);
}
int RxItem2D::MouseOnItem(int mx, int my)
{
	int winHeight,winWidth, winPosX,winPosY;
	
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	winPosX   = viewport[0];
	winPosY   = viewport[1];
	winWidth  = viewport[2];
	winHeight = viewport[3];

    //Attention : origine Opengl dans le coin inférieur gauche et origine de la fenetre dans le coin superieur gauche
    int  my_to_gl;
    my_to_gl = (winHeight)-my;
	
	//correction des coordonnées souris par rapport à la position du viewport
	my_to_gl-=winPosY;
	mx -=winPosX;

    //on verifie si la souris se trouve bien sur l'objet en question
    if( (mx >= getPosX()) && (mx <= (getPosX()+getWidth())) && (my_to_gl >= getPosY()) && (my_to_gl <= (getPosY()+getHeight())) )
       return true;

    return false; //souris en en dehors de l'objet
}

void RxItem2D::CreateInstanceVBOShader()
{

	#ifdef _DEBUG
		RXCHECKGLERROR("[RxItem2D][CreateInstanceVBOShader] 0");
	#endif

	static RxVBO vbo;
	pvbo = &vbo;

	if(!vbo.IsValide())
	{
		vbo.Destroy();

		glm::vec2 A,B,C,D;
		glm::vec2 At,Bt,Ct,Dt;

		A = glm::vec2(0.0f,0.0f);						//0
		B = glm::vec2(1.0f,0.0f);				//1
		C = glm::vec2(1.0f,1.0f);		//2
		D = glm::vec2(0.0f,1.0f);	

		At = glm::vec2(0.0f,0.0f);						//0
		Bt = glm::vec2(1.0f,0.0f);				//1
		Ct = glm::vec2(1.0f,1.0f);		//2
		Dt = glm::vec2(0.0f,1.0f);				//3

		std::vector<glm::vec2> vertex;
		std::vector<glm::vec2> texcoords;

		//methodes glDrawElements
		vertex.push_back(A); texcoords.push_back(At);	//0
		vertex.push_back(B); texcoords.push_back(Bt);	//1
		vertex.push_back(C); texcoords.push_back(Ct);	//2
		vertex.push_back(D); texcoords.push_back(Dt);	//3


		const unsigned int indices[] = {0,1,2,   //triangle 1
										0,2,3};	 //triangle 2




		vbo.AddArrayBufferData(RX_VERTEX_ATTRIB_INDEX,vertex);
		vbo.AddArrayBufferData(RX_COORD0_ATTRIB_INDEX,vertex);

		vbo.AddElementBufferData(indices,6);



		if(!vbo.Create(GL_STATIC_DRAW))
			RXLOGGERERROR("[RxItem2D][CreateInstanceVBOShader] vbo create error");
	}



	static RxShader defaultShader;

	#ifdef _DEBUG
	RXCHECKGLERROR("[RxItem2D][CreateInstanceVBOShader] 000");
	#endif

	pshader = &defaultShader;
	if(!defaultShader.IsValid())
	{
		defaultShader.Load(RX_VERT_SHADER_ITEM2D, RX_FRAG_SHADER_ITEM2D);
	}
	#ifdef _DEBUG
		RXCHECKGLERROR("[RxItem2D][CreateInstanceVBOShader] 1");
	#endif
}

void RxItem2D::Draw()
{
    if (!visible || pshader == NULL) return; //on ne dessine pas si non visible


	

    //glColor3f(R,G,B); //couleur de l'objet
    // si l'objet possède une texture on l'active
    if (idTex > 0)
        glBindTexture(GL_TEXTURE_2D, idTex);
    else
        glBindTexture(GL_TEXTURE_2D, 0);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT,viewport);
	glm::mat4 projection = glm::ortho(0.0f, (float) viewport[2]-viewport[0] ,0.0f, (float)viewport[3]-viewport[1]);

	RxShader *vShader;
	if(ptrShader)
		vShader = ptrShader;
	else
		vShader = pshader;

	vShader->Enable();
	vShader->SendMat4f("projection",GL_FALSE,projection);
	vShader->Send1i("textureSampler", 0);
	vShader->Send4f("uColor",glm::vec4(R,G,B,A));
	vShader->Send2i("offset",getPosX(),getPosY());
	vShader->Send2i("scale",getWidth(),getHeight());
	pvbo->Enable();
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
	pvbo->Disable();

    if( (font != NULL)&&(!text.empty()) )
	{	
		font->setTextColor(glm::vec4(R_text,G_text,B_text,A_text));
		font->PrintText(getPosX()+text_x,getPosY()+text_y,text.data());
	}
 
	vShader->Disable();
}

void RxItem2D::setCallBackFct( void(*fct)(int,RxItem2D*))
{
	fctCallBack = fct;
}
int RxItem2D::CallBackCoord()
{

   //si l'objet n'est pas visible on ne fait rien
   if (!visible)
   {
	   if(fctCallBack) fctCallBack(-1,this);
	   return -1;
   }

   int mx,my,_bouton;

   //relève l'état de la sourie (bouton enfoncé et position)
   _bouton = SDL_GetMouseState(&mx, &my);


   //attribue le focus à l'ITEM si on vient juste de cliquer dessus.
   if (MouseOnItem(mx, my) && (_bouton == bouton_enable) && (lastBouton !=bouton_enable) )
   focus = ID;


   // si l'item a le focus alors les envenements s'applique à celui-ci
   if(focus == ID)
   {

           //la sourie n'est pas sur l'objet et aucun bouton n'a été enfoncé, on ne fait rien
           //"if" servant pour le glissé de l'objet, en effet il est possible que lors du glissé de l'item
           //la souris sort de l'item sans que le bouton de la souris soit relachée, on perd le focus
           if(!MouseOnItem(mx, my) && !(lastBouton ==bouton_enable) )
           {
                lastBouton = _bouton;
                lastMouse_x = mx;
                lastMouse_y = my;
                focus = -1;
				if(fctCallBack) fctCallBack(-1,this); //appelle de la fct callback
                return -1;
           }

           //sourie sur l'objet et bouton actif enfoncé : on peut bouger l'objet
           if( (movable) && (_bouton == bouton_enable))//que s'il est possible de bouger l'item
           {
					   int dx,dy;
                       dx = getPosX() + (mx-lastMouse_x);
                       dy = getPosY() - (my-lastMouse_y); //on soustrait car l'axe y est opposé entre opengl et sdl.
					   setPosition(dx,dy);
            }

               if(!outside)ItemStayWithFather();

               lastBouton = _bouton;
               lastMouse_x = mx;
               lastMouse_y = my;
             
			   if(fctCallBack) fctCallBack(_bouton,this);
               return _bouton;


   }
   else if(MouseOnItem(mx, my)) //si on est sur l'objet
   {
        lastBouton = _bouton;
        lastMouse_x = mx;
        lastMouse_y = my;
		if(fctCallBack) fctCallBack(0,this);
        return 0;
   }



           lastBouton = _bouton;
           lastMouse_x = mx;
           lastMouse_y = my;
        
		   if(fctCallBack) fctCallBack(-1,this);
           return -1;

}



void RxItem2D::ItemStayWithFather()
{
     //on verifie bien que les dimensions de l'objet mère ont bien été initialisé
    int fatherWidth,fatherHeight,fatherX,fatherY;
	if ( father == NULL )
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		fatherX = 0;
		fatherY = 0;
		fatherWidth = viewport[2];
		fatherHeight = viewport[3];

	}
	else
	{
		fatherHeight = father->getHeight();
		fatherWidth = father->getWidth();
		fatherX = father->getPosX();
		fatherY = father->getPosY();
	}
		
	 int dx = getPosX(),dy = getPosY();
     if (getPosX() < fatherX) dx = fatherX;
	 if (getPosY() < fatherY) dy = fatherY;
	// setPosition(dx,dy);


	 if ( (getPosX() + getWidth() ) > (fatherWidth)+fatherX)
         dx = fatherWidth-getWidth()+fatherX;

     if ( (getPosY()+getHeight()) > (fatherHeight+fatherY))
         dy = fatherHeight-getHeight()+fatherY;

	 setPosition(dx,dy);

}
RxItem2D * RxItem2D::Copy()
{
	//création d'une copie de l'objet et retourne le pointeur associé
	RxItem2D * copyitem = new RxItem2D;
	(*copyitem) = (*this);
	return copyitem;
}
/*************************************************************************/
/*** CLASSE RxButton *****************************************************/
RxButton::RxButton():RxItem2D()
{
	type = RX_BUTTON;
    idTexMouseOut = 0;
    idTexMouseOn  = 0;
    idTexMouseClick   = 0;

    R_MouseOut = G_MouseOut = B_MouseOut = 1.0f;
    R_MouseOn = G_MouseOn = B_MouseOn = 1.0f;
    R_MouseClick = G_MouseClick = B_MouseClick = 1.0f;

    movable = 0;
}

RxButton::~RxButton()
{


    //Dans la classe RxButton, "text" est un pointeur vers l'un des 3 textes possibles
    //du bouton. Or l'espace mémoire indiqué par "text" a déjà été surprimé par le destructeur de RxButton
    //Cependant si "text" n'est pas à NULL, RxItem2D va denouveau cette espace déjà libérer.
    //On met donc "text" à NULL pour éviter toute erreur de over-suppression.
    //PS : C++ appelle les destructeurs dans l'ordre inverse à la construction des objets.
    //C'est à dire que les destructeurs des classes filles(RxButton) sont appelés avant le
    //destructeurs de la classe mère(RxItem2D)
    text.clear();
}

void RxButton::setIDsTex( int idtexmouseout, int idtexmouseon, int idtexmouseclick )
{
    idTexMouseOut = idtexmouseout;

    if( !idtexmouseon ) 
		idTexMouseOn = idTexMouseOut;
    else 
		idTexMouseOn = idtexmouseon;

    if( !idtexmouseclick )
		idTexMouseClick = idTexMouseOut;
    else 
		idTexMouseClick = idtexmouseclick;




    //valeur par défaut
    idTex = idTexMouseOut;

}
void RxButton::setColors
(
    float r_out     , float g_out     , float b_out, float a_out,
    float r_on      , float g_on      , float b_on, float a_on,  
	float r_click  , float g_click  , float b_click, float a_click
)
{

    R_MouseOut=R_MouseOn=R_MouseClick=R=r_out;
    G_MouseOut=G_MouseOn=G_MouseClick=G=g_out;
    B_MouseOut=B_MouseOn=B_MouseClick=B=b_out;
	A_MouseOut=A_MouseOn=A_MouseClick=A=a_out;

    if( r_on >= 0)
    {
        R_MouseOn = r_on;
        G_MouseOn = g_on;
        B_MouseOn = b_on;
		A_MouseOn = a_on;
    }

    if( r_click >= 0)
    {
        R_MouseClick = r_click;
        G_MouseClick = g_click;
        B_MouseClick = b_click;
		A_MouseClick = a_click;
    }

}

#ifdef _RXMATH3D_H_
void RxButton::setColors( glm::vec4 rgb_out, glm::vec4 rgb_on, glm::vec4 rgb_click )
{
	setColors(
		rgb_out.r, rgb_out.g, rgb_out.b, rgb_out.a,
		rgb_on.r, rgb_on.g , rgb_on.b, rgb_on.a,
		rgb_click.r, rgb_click.g, rgb_click.b, rgb_click.a
		
		);
}
void RxButton::setColors( glm::vec3 rgb_out, glm::vec3 rgb_on, glm::vec3 rgb_click )
{
	setColors(
		rgb_out.r, rgb_out.g, rgb_out.b, 1.0f,
		rgb_on.r, rgb_on.g , rgb_on.b, 1.0f,
		rgb_click.r, rgb_click.g, rgb_click.b, 1.0f
		);
}


#endif
void RxButton::setTexts(const char* tOut, const char* tOn, const char* tClick)
{
   if( tOut != NULL )
   {
       //on supprime le texte déjà présent
       textMouseOut.clear();
       textMouseOut = tOut;
   }

   if( tOn != NULL )
   {
       //on supprime le texte déjà présent
       textMouseOn.clear();
       textMouseOn = tOn;
   }

   if( tClick != NULL)
   {
       //on supprime le texte déjà présent
       textMouseClick.clear();
       textMouseClick = tClick;
   }
   if(text.empty())
    text = textMouseOut;
}

int RxButton::CallBackCoord()
{

    //variable sauvegardant l'état de la souris par rapport au bouton
    //correspondant à la valeur renvoyer par la fonction CallBackCoord de ITEM2D
    int GraphicsState;
    GraphicsState = RxItem2D::CallBackCoord();
    //bouton souris en dehors du bouton
    if ( GraphicsState == -1)
    {
         idTex = idTexMouseOut;
         text = textMouseOut;
         R = R_MouseOut;
         G = G_MouseOut;
         B = B_MouseOut;
		 A = A_MouseOut;
    }
    //souris sur le bouton
    else if ( GraphicsState == 0)
    {
         idTex = idTexMouseOn;
         text = textMouseOn;
         R = R_MouseOn;
         G = G_MouseOn;
         B = B_MouseOn;
		 A = A_MouseOn;
    }
    //souris sur le bouton avec clique
    else
    {
         idTex = idTexMouseClick;
         text = textMouseClick;
         R = R_MouseClick;
         G = G_MouseClick;
         B = B_MouseClick;
		 A = A_MouseClick;
    }

    //verification des valeurs par défaut
    if( !idTex ) idTex = idTexMouseOut;
    if( text.empty()) text = textMouseOut;



    return GraphicsState;
}

RxItem2D * RxButton::Copy()
{
	RxButton* copyitem  =  new RxButton;
	(*copyitem) = (*this);

	return (RxItem2D*) copyitem;
}
/***************************************************************************/
/*** CLASSE RxTextBox *****************************************************/

RxTextBox::RxTextBox():RxItem2D()
{
	type = RX_TEXTBOX;
	width = 50;
	height = 50;

    BUp.setSize(RX_BTN_WIDTH,RX_BTN_HEIGHT);
    BUp.setColors(1,0,0,1,1,0,1,0,1);

    BDown.setSize(RX_BTN_WIDTH,RX_BTN_HEIGHT);
    BDown.setColors(0,1,0,1,1,0,0,0,1);

    BScroll.setSize(RX_BTN_WIDTH,height-RX_BTN_HEIGHT*2);
    BScroll.setColors(0,0,1,1,0,1,0,1,1);
	
    BOnScroll.setSize(RX_BTN_WIDTH,RX_BTN_HEIGHT);
    BOnScroll.setColors(0.3f,0.3f,0.3f,0.6f,0.6f,0.6f,0.9f,0.9f,0.9f);
	BOnScroll.setText("|");
	BOnScroll.setFather(&BScroll);
	BOnScroll.setOutside(0);
	BOnScroll.setMovable(1);
	
    BUp.setTexts("Up1","Up2","Up3");
    BDown.setTexts("Down1","Down2","Down3");

    nb_lines = 0;
    nb_lines_to_shows = 0;
    line_start = 1;
    border = 1;
    setColor(0.1f,0.1f,0.3f);
    fps = NULL;
    speed_text = 0.015f;
}
//en commentaire, même travail que le constructeur généré automatiquement
RxTextBox& RxTextBox::operator=(RxTextBox& item)
{
	RxItem2D::operator=(item);
	BOnScroll = item.BOnScroll;
	BUp = item.BUp;
    BDown = item.BDown;
    BScroll =item.BScroll;
	BOnScroll.setFather(&BScroll);
	setPosition(item.getPosX(),item.getPosY());
	
	this->setText( item.original_text.c_str());
    
	nb_lines = 0;
    nb_lines_to_shows = 0;
    line_start = 1;
	this->setBorder(item.border);
    fps = item.fps;
    speed_text = item.speed_text;
	


	return(*this);
}
RxTextBox::RxTextBox(RxTextBox & item):RxItem2D(item)
{
	
	type = RX_TEXTBOX;
	BOnScroll = item.BOnScroll;
	BUp = item.BUp;
    BDown = item.BDown;
    BScroll =item.BScroll;
	setPosition(item.getPosX(),item.getPosY());
	
	original_text = item.original_text;
	format_text = item.format_text;
    nb_lines = item.nb_lines;
    nb_lines_to_shows = item.nb_lines_to_shows;
    line_start = item.line_start;
    border = item.border;
    fps = item.fps;
    speed_text = item.speed_text;
}
RxTextBox::~RxTextBox()
{

}
void RxTextBox::setFather( RxItem2D *_father )
{
	this->father = _father;
    BUp.setFather(_father);
    BDown.setFather(_father);
    BScroll.setFather(_father);
}
void RxTextBox::setBorder(int b)
{
    border = b;
    KeepTextInBox();
}
void RxTextBox::setSpeedScroll(float speed)
{
	speed_text = speed;
}
void RxTextBox::setFont(RxFont *_f)
{
    font = _f;
    BUp.setFont(font);
    BDown.setFont(font);
    BScroll.setFont(font);
}
void RxTextBox::setPosition(int _x, int _y)
{
   	int dx,dy;
	dx = _x-pos_x;
	dy = _y-pos_y;

    
	pos_x = _x;
    pos_y = _y;

	//position des boutons
    BUp.setPosition(pos_x+width,pos_y+height-RX_BTN_HEIGHT);
    BDown.setPosition(pos_x+width,pos_y);
    BScroll.setPosition(pos_x+width,pos_y+RX_BTN_HEIGHT);
	
	BOnScroll.setPosition(dx+BOnScroll.getPosX(),dy+BOnScroll.getPosY());	
	

}

void RxTextBox::setSize( int _w, int _h)
{
    width = _w;
    height = _h;

    //definition des objets
    BUp.setSize(RX_BTN_WIDTH,RX_BTN_HEIGHT);
    BDown.setSize(RX_BTN_WIDTH,RX_BTN_HEIGHT);
    BScroll.setSize(RX_BTN_WIDTH,height-RX_BTN_HEIGHT*2);
	BOnScroll.setSize(RX_BTN_WIDTH,RX_BTN_HEIGHT);
	BOnScroll.setPosition(pos_x+width,pos_y+height-2*RX_BTN_HEIGHT);
	
	setPosition(pos_x,pos_y);

    KeepTextInBox();
}

void RxTextBox::setText(const char *t)
{
    //on a passé un pointeur vide on ne fait rien
     if (t == NULL) return;
     original_text=t;
	 KeepTextInBox();
	 int i;
	 i = t[0];
}

void RxTextBox::setFpsPointer(unsigned int *_fps){ fps =  _fps;}
void RxTextBox::setSpeed(float sv){ speed_text = sv; }

void RxTextBox::MoveTextWithScoll()
{
		int relative_y = BOnScroll.getPosY()-BScroll.getPosY();

		int range_pixel_scroll = BScroll.getHeight() - BOnScroll.getHeight(); //zone dans laquelle le bouton peut bouger
		int range_line_scroll = nb_lines-nb_lines_to_shows; //intervalle de ligne à répartir sur range_mov
		
		float nb_pixel_by_line = (float) range_pixel_scroll/(float)range_line_scroll; //nombre de pixel par ligne
		
		//recherche de la ligne à afficher en fonction de la position
		line_start = (int)(range_line_scroll-(relative_y/nb_pixel_by_line)+1); //inversion par rapport à l'axe des abysses (le texte défile de haut en bas et non de bas en haut)
}

void RxTextBox::MoveScroll()
{
	int range_pixel_scroll = BScroll.getHeight() - BOnScroll.getHeight(); //zone dans laquelle le bouton peut bouger
	int range_line_scroll = nb_lines-nb_lines_to_shows; //intervalle de ligne à répartir sur range_mov
	float nb_pixel_by_line = (float) range_pixel_scroll/(float)range_line_scroll; //nombre de pixel par ligne

	int relative_y = (int) ((range_line_scroll+1-line_start)*nb_pixel_by_line);

	int pos_y = relative_y + BScroll.getPosY();
	BOnScroll.setPosition(BOnScroll.getPosX(),pos_y);
}
int RxTextBox::CallBackCoord()
{
    if(!visible)
	{	
		if(fctCallBack) fctCallBack(-1,this);
		return -1;
	}


    int mov = false;
    int buttonused = false;
	int no_scroll = false;
    if( BUp.CallBackCoord()>0 )
    {
		 buttonused = true;
         if( fps != NULL)
         {
            static float cp = 0;
            cp += (speed_text*(*fps));

            if(cp>=1)
            {
                line_start--;
                mov = true;
				no_scroll = true;
                cp = 0;
            }
         }
         else
         {
             line_start--;
             mov = true;
			 no_scroll = true;
         }
    }
    else if( BDown.CallBackCoord()>0 )
    {
		 buttonused = true;
         if( fps != NULL)
         {
            static float cp = 0;
            cp += (speed_text*(*fps));

            if(cp>=1)
            {
                line_start++;
                mov = true;
				no_scroll = true;
                cp = 0;
            }
         }
         else
         {
             line_start++;
             mov = true;
			 no_scroll = true;
         }
    }
	
	//a definir le fonctionnement
    else if(BOnScroll.CallBackCoord()>0)
	{
		MoveTextWithScoll();
		mov = true;
		buttonused = true;
		
	//a definir le fonctionnement
	}
    else if(BScroll.CallBackCoord()>0)
	{
		buttonused = true;
	}

    //evite les débordement du texte.
    if(line_start > nb_lines-nb_lines_to_shows)
    {
        line_start = nb_lines-nb_lines_to_shows+1;
    }
    if(line_start < 1) line_start = 1;

	if(no_scroll)
		MoveScroll();


    // si un des bouton de la textbox a été préssé,
    // on recalcule le texte à afficher.
    if( mov) CpyVisibleLinesToText();
   
	if(!buttonused)
		return RxItem2D::CallBackCoord();
	else
		return 1;
}


void RxTextBox::CreateTextByline()
{
	//s'il y a pas de font et pas de texte on ne fait rien
	if(font == NULL || original_text.empty() )return;

	//place la position du texte en haut de la zone de texte
    text_x = border;  
	text_y = height - font->getLineSkip();

	std::string BUFFER_LINE;
	BUFFER_LINE.reserve(original_text.size());

	unsigned int k;
	for( k = 0; k < original_text.size(); k++)
	{

	}

}

void RxTextBox::KeepTextInBox()
{
	
	//fonction buggée à corriger de la librairie TTF_FONT.


	if(font == NULL || original_text.empty() )return;
    //place la position du texte en haut de la zone de texte
	//et en prenant en compte la bordure
    text_y = height - font->getLineSkip();
    text_x = border;

    //transforme le texte d'origine en texte formaté pour la textbox
	std::string BUFFER_TEXT; 
	BUFFER_TEXT.reserve(original_text.size());
    int k,end,last_space_original,last_line_buffer;

    end = original_text.size();
    last_space_original = 0; //Dernier espace trouvé dans le texte original
    last_line_buffer = 0; //dernier ligne trouvé sur le buffer
 	int offset = 0; //offset sur la chaine BUFFER par rapport à original_text
					//du à l'ajout d'espace pr les mots trop longs

    nb_lines = 1; //on a automatiquement une ligne

    //rajout de ligne
    for (k = 0; k < end; k++)
    {
        //on copie le texte original dans le buffer
        BUFFER_TEXT.push_back(original_text[k]);
      

        //on relève la position du dernier espace rencontré
		//pour pouvoir y revenir et le remplacer par un retour à la ligne en cas de besoin
        if(original_text[k] == ' ') last_space_original = k;

		//copie de la dernière ligne contenu dans buffer à mesurer
		std::string tmp = BUFFER_TEXT.c_str()+last_line_buffer; 
		const char *ptrtest = tmp.c_str();
		//si la ligne est trop grande on la découpe
        if(font->LengthText(tmp.c_str()) > width-2*border)   
        {
			if(last_space_original) //dans le cas ou il existe un espace dans la dernière ligne
			{
				//replacement de la dernière ligne devant le dernier espace (last_space_original+1), 
				//attention à ne pas oublier l'offset entre le texte original et le buffer (+offset)
				last_line_buffer = last_space_original+offset+1; 
				nb_lines++;//on a une ligne de plus
				BUFFER_TEXT.resize(	BUFFER_TEXT.size() - (k-last_space_original)-1); //suppression des lettres qui ont été copié après l'espace
				BUFFER_TEXT.push_back('\n');	//rajoute un saut de ligne à la place de l'espace
				k = last_space_original; // on repart de l'ancien espace
				last_space_original = 0; //l'espace a été utilisé on ne peut pas le réutiliser (cas des mots trop long par rapport à la boite)
			}
			else if(tmp.size()>1) //mot trop long par rapport à la largeur de la boite avant le prochain espace. on le découpe en deux s'il ne se résume pas à 1 caractère
			{
				offset++; //rajout d'un caractère par rapport à original_text
				last_line_buffer = k+offset; //trop long on revient un cran en arrière
				BUFFER_TEXT[BUFFER_TEXT.size()-1] ='\n'; //on coupe le mot en sautant une ligne
				k --;
				nb_lines++;//on a une ligne de plus
			}
        }

    }

    format_text.clear();
    format_text = BUFFER_TEXT;

   
    text.clear();
    text = format_text;
    //calcul le nombre de ligne qu'il est possible d'afficher
    nb_lines_to_shows = (height-2*border) / font->getLineSkip();
    line_start = 1;
    CpyVisibleLinesToText();

}

void RxTextBox::CpyVisibleLinesToText()
{
    if( (format_text.empty()) || (text.empty()) ) return;
    unsigned int k,k2 = 0;
    int c_line = 1;
	text.clear();
	//on parcourt l'ensemble des données
    for(k = 0; k < format_text.size(); k++)
    {
		
		//si on a copié le nombre de ligne affichable on arrete
		if( (c_line - line_start>=nb_lines_to_shows) )
		{
			break;
		}
        else if( (c_line >= line_start) ) //on verifié de commencer la copie à la bonne ligne
        {
			text.push_back(format_text[k]);
            //k2++;
        }

		//on vient de passer à la ligne suivante.
        if(format_text[k]=='\n') c_line++;
    }
   // text[k2] = '\0';
}
void RxTextBox::Draw()
{
    if(!visible) return;
	

	
    //dessins des boutons
    BDown.Draw();
    BUp.Draw();
    BScroll.Draw();
	BOnScroll.Draw();
	// dessin du font
    this->RxItem2D::Draw();
}

RxItem2D * RxTextBox::Copy()
{

	RxTextBox* copyitem  =  new RxTextBox;
	(*copyitem) = (*this);
    
	return (RxItem2D*) copyitem;
}
int RxTextBox::getRealWidth()
{
	return getWidth()+ BScroll.getWidth();
}
void RxTextBox::ItemStayWithFather()
{
     //on verifie bien que les dimensions de l'objet mère ont bien été initialisé
    int fatherWidth,fatherHeight,fatherX,fatherY;
	if ( father == NULL )
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		fatherX = 0;
		fatherY = 0;
		fatherWidth = viewport[2];
		fatherHeight = viewport[3];

	}
	else
	{
		fatherHeight = father->getHeight();
		fatherWidth = father->getWidth();
		fatherX = father->getPosX();
		fatherY = father->getPosY();
	}
		
	 int dx = getPosX(),dy = getPosY();
     if (getPosX() < fatherX) dx = fatherX;
	 if (getPosY() < fatherY) dy = fatherY;
	// setPosition(dx,dy);


	 if ( (getPosX() + getRealWidth() ) > (fatherWidth+fatherX))
         dx = fatherWidth-getRealWidth()+fatherX;

     if ( (getPosY()+getHeight()) > (fatherHeight+fatherY))
         dy = fatherHeight-getHeight()+fatherY;

	 setPosition(dx,dy);

}
/*************************************************************************/
/*** CLASSE RxWindow *****************************************************/


RxWindow::RxWindow()
{
	type = RX_WINDOW;
}

RxWindow::~RxWindow()
{
	//on oublie pas supprimer les objets créés
	Clear();
}

void RxWindow::Clear()
{
	unsigned int i;
	//suppression des objets créés
	for(i = 0; i < itemlist.size(); i++)
	{
		delete itemlist[i];
		itemlist[i] = NULL;
	}
	itemlist.clear();

}

RxItem2D* RxWindow::AddLabel()
{
	RxItem2D *newItem = new RxItem2D;
	newItem->setFather(this);
	newItem->setOutside(0);
	itemlist.push_back(newItem);
	return newItem;
}
RxItem2D* RxWindow::AddButton()
{
	RxItem2D *newItem = (RxItem2D *)new RxButton;
	newItem->setFather(this);
	newItem->setOutside(0);
	itemlist.push_back(newItem);
	return newItem;
}
RxItem2D* RxWindow::AddTextbox()
{
	RxItem2D *newItem = (RxItem2D *)new RxTextBox;
	newItem->setFather(this);
	newItem->setOutside(0);
	itemlist.push_back(newItem);
	return newItem;
}

RxItem2D* RxWindow::AddItem(RxItem2D *item)
{
	RxItem2D *cpy = item->Copy();
	cpy->setFather(this);
	cpy->setOutside(0);
	itemlist.push_back(cpy);
	return cpy;
}

void RxWindow::setPosition(int _x, int _y)
{	
	//calcul le déplacement de pixel
	int dx = _x-pos_x;
	int dy = _y-pos_y;

	pos_x = _x;
	pos_y = _y;


	unsigned int i;
	//incrémente la position des objets de fenetre du déplacement effectué par celle-ci
	//Afin de les faire bouger avec la fenetre.	
	for(i = 0; i < itemlist.size(); i++)
	{
		int x,y;
		x = dx+ itemlist[i]->getPosX();
		y = dy+ itemlist[i]->getPosY();
		itemlist[i]->setPosition(x,y);
	}
}



void RxWindow::setFont(RxFont *_f, int n)
{
	if(n<0)
	{
		
		this->font = _f;
		unsigned int i;
		for(i = 0; i < itemlist.size(); i++)
			itemlist[i]->setFont(_f);
	}
	else if( n< (int)itemlist.size())
	{
		itemlist[n]->setFont(_f);
	}
}

int RxWindow::CallBackCoord()
{
	unsigned int i;
	int val;
	for(i = 0; i < itemlist.size(); i++)
	{	
		if( (val=itemlist[i]->CallBackCoord())>0) return val; //renvoie la valeur
	}

	return RxItem2D::CallBackCoord();
}



/*variable jouant sur la vitesse de defilement du texte
lorque l'on appuie sur les bouton de bas/haut**/

RxItem2D * RxWindow::Copy()
{
	RxWindow *newItem = new RxWindow;
	
	//copie des variables de type non dynamique
	//attention copie des pointeurs aussi
	(*newItem)=(*this);

	//recreation et copy des items dynamique de la fenetre 

	//suppression des pointeurs de la fenetre copie
	//(ne pas appeler la méthode RxWindows::Clear() 
	//qui supprimerai les instances des objets de this )
	newItem->itemlist.clear(); 

	//création d'une copie des objets de la fenetre.
	unsigned int i;
	for(i = 0; i < this->itemlist.size(); i++)
	{
		RxItem2D* cpy = this->itemlist[i]->Copy(); //création d'une copie de l'objet
		newItem->itemlist.push_back(cpy); //rajout de l'instance de l'objet copié
	}

	//on retourne l'objet copié
	return newItem;
}
void RxWindow::Draw()
{
	//Dessine le fond de la fenetre
	this->RxItem2D::Draw();

	//Dessine tous les objets de la fenetre
	unsigned int i;
	for(i = 0; i < itemlist.size(); i++)
	{
		itemlist[i]->Draw();
	}



}
std::vector<RxItem2D*> &RxWindow::getItemlist()
{
	return itemlist;
}
