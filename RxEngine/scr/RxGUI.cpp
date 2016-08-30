
#include "RxGUI.h"


RxGUI::RxGUI()
{
		//valeurs par defaut des objets
		defaultitem.setColor(1.0f,1.0f,1.0f);
		defaultitem.setColorText(0.3f,0.3f,0.3f);
		defaultitem.setMovable(1);
		defaultitem.setOutside(0);
		defaultitem.setPosText(10,10);
		defaultitem.setSize(70,30);
		defaultitem.setText("none");
		defaultitem.setVisible(1);
		defaultitem.setFont(NULL);
}
RxGUI::~RxGUI()
{
	Release();
}

void RxGUI::Release()
{
	unsigned int i;
	for(i = 0; i < tabItem2D.size(); i++)
		tabItem2D[i].release();
}

void RxGUI::Draw()
{
	unsigned int i;
	for(i = 0; i < tabItem2D.size(); i++)
		tabItem2D[i].item->Draw();
}

RxItem2D* RxGUI::AddItem2D(RxItem2D &item)
{
	return NULL;
}

void RxGUI::CallBackAllFct()
{
	unsigned int i;
	for(i = 0; i < tabItem2D.size(); i++)
	{
		int val = tabItem2D[i].item->CallBackCoord();
		if(tabItem2D[i].callBackFct != NULL)
			(tabItem2D[i].callBackFct)(val);
	}
}

void RxGUI::setCallBactFct(std::string itemname, void (*callBackFct)(int))
{
	unsigned int i = 0;
	for(i=0; i < tabItem2D.size(); i++)
	{
		if(tabItem2D[i].name == itemname)
		{
			tabItem2D[i].callBackFct = callBackFct;
			return;
		}
	}
}
void RxGUI::setCallBactFct(RxItem2D *item, void (*callBackFct)(int))
{
	unsigned int i = 0;
	for(i=0; i < tabItem2D.size(); i++)
	{
		if(tabItem2D[i].item == item)
		{
			tabItem2D[i].callBackFct = callBackFct;
			return;
		}
	}
}
void RxGUI::setCallBactFct(int itemid, void (*callBackFct)(int))
{
	unsigned int i = 0;
	for(i=0; i < tabItem2D.size(); i++)
	{
		if(tabItem2D[i].id == itemid)
		{
			tabItem2D[i].callBackFct = callBackFct;
			return;
		}
	}
}

int RxGUI::LoadXMLGUI(const char *_filename)
{
	
	if (_filename == NULL)
	{
		RXLOGGERERROR("[RxGUI][LoadXMLGUI] _filename == NULL");
		return RX_ERROR;
	}
	
	std::string filename = _filename;

	TiXmlDocument doc(filename.c_str());
	if(!doc.LoadFile()){ //erreur d'ouverture
		RXLOGGERERROR("[RxGUI][LoadXMLGUI] Fail on file : "<<filename);
		RXLOGGERERROR("[RxGUI][LoadXMLGUI] Log :" << doc.ErrorId() << " : " << doc.ErrorDesc());
	    return RX_ERROR;
	}

	
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(NULL);
	const char *ptr;
		
		//block rxgui : lecture du nom du GUI
		{
				pElem=hDoc.FirstChildElement().Element();
				// should always have a valid root but handle gracefully if it does
				if (!pElem) return 0;
				if(ptr = pElem->Attribute("name") )
					name = ptr; //enregistre le nom de la scene
				// save this for later
				hRoot=TiXmlHandle(pElem);
		}

//////////////////////////////////////////////////////////////////////////////////////
		// block texturelist: lecture et chargement des images
//////////////////////////////////////////////////////////////////////////////////////
		
		
		{

			pElem=hRoot.FirstChild( "texturelist" ).FirstChildElement().Element();
			for( pElem; pElem; pElem=pElem->NextSiblingElement()) //pour chacune des textures
			{
			
				std::string value = pElem->Value();
				if(value.compare("texture") == 0)
				{
					//relève les infos de la texture
					std::string texFile;
					if(ptr = pElem->Attribute("filename"))
					  texFile = ptr;
					
					idXML2idGL id;
					pElem->QueryIntAttribute("id", &id.idxml);
					
					//enregistre les données et charge la texture
					id.idgl = RxTexManager::Instance().LoadTexture(texFile.data());
					
					idtextab.push_back(id);
				}
				else if(value.compare("multitexture")==0)
				{
					//relève les infos de la texture
					std::string name;
					if(ptr = pElem->Attribute("name"))
					  name = ptr;
					
					glm::ivec2 format(0,0);
					if(ptr = pElem->Attribute("format"))
						format = str2ivec2(ptr);

					idXML2idGL id;
					pElem->QueryIntAttribute("id", &id.idxml);

					std::vector<std::string> filenames;
					//relève les textures
					TiXmlElement * pElem2 = pElem->FirstChildElement();
					for( pElem2; pElem2; pElem2=pElem2->NextSiblingElement()) //pour actor
					{
						if(ptr = pElem2->Attribute("filename"))
						{
							filenames.push_back(ptr);
						}
					}
					
					if( (format.x <= 0) || (format.y <= 0) )
					{
						int d = (int)sqrt((float)filenames.size());
						if( d*d == filenames.size())
							format = glm::ivec2(d,d);
						else
							format = glm::ivec2(d+1,d+1);
					}

					format.x = RxPowerOfTwo(format.x);
					format.y = RxPowerOfTwo(format.y);

    
					//enregistre les données et charge la texture
					id.idgl = RxTexManager::Instance().CreateMultiTexture(filenames,format.x, format.y,name.c_str());
					
					idtextab.push_back(id);
				}
				
			} 

		}//fin bloc texturelist


//////////////////////////////////////////////////////////////////////////////////////
		// block itemlist: lecture et chargement des objets
//////////////////////////////////////////////////////////////////////////////////////
		{
			
			pElem=hRoot.FirstChild( "itemlist" ).FirstChildElement().Element();
			for( pElem; pElem; pElem=pElem->NextSiblingElement()) //pour chacun des objet
			{
				RxItem2D* item = NULL;

				//relève le nom de l'objet
				std::string itemname("none");

				if(ptr = pElem->Attribute("name"))
					itemname = ptr;

				std::string value = pElem->Value(); //relève le type d'objet
				//en fonction de l'objet
				if(value.compare("default") == 0)
				{
					//relève données de l'ojet par défaut
					ItemReader(&defaultitem,pElem);
					continue; //on passe à l'iteration suivante pr ne pas rajouter l'élément par defaut à la liste des objets
				}
				else if(value.compare("item2d")==0)
				{
					item = new RxItem2D; //creation de notre objet
					//on applique les valeurs par defaut
					setDefaultValues(item);
					//lecture de l'objet
					ItemReader(item,pElem);
				}//fin item
				else if(value.compare("button")==0)
				{
					item = new RxButton; //creation de notre objet
					//on applique les valeurs par defaut
					setDefaultValues(item);
					//lecture de l'objet
					ButtonReader((RxButton*)item,pElem);
				}
				else if(value.compare("textbox")==0)
				{
					item = new RxTextBox(); //creation de notre objet
					
					//on applique les valeurs par defaut
					setDefaultValues(item);
					//lecture de l'objet
					TextBoxReader((RxTextBox*)item, pElem);
					
				}
				
				//rajoute l'objet à la liste
				RxControler crtler;
				crtler.item = item;	
				crtler.id = tabItem2D.size();
				crtler.name=itemname;
				tabItem2D.push_back(crtler);

			} //fin for

		}//fin bloc itemlist


	return RX_OK;
}
void RxGUI::setFont( RxFont *_font)
{

	font = _font;
	defaultitem.setFont(font);
	unsigned int i;
	for(i=0; i < tabItem2D.size();i++)
	{
		tabItem2D[i].item->setFont(_font);
	}

}
void RxGUI::setFather(RxItem2D* father)
{
	defaultitem.setFather(father);
	unsigned int i;
	for(i=0; i < tabItem2D.size();i++)
	{
		tabItem2D[i].item->setFather(father);
	}
}
void RxGUI::setDefaultValues(RxItem2D *item)
{
	(*item) = (defaultitem);

}
bool RxGUI::GlobalItemValues( RxItem2D *item, TiXmlElement * pElem2 )
{
	const char *ptr;
	std::string type = pElem2->Value();
	if(type == "position")
	{
		if(ptr = pElem2->Attribute("value"))
			 item->setPosition(str2ivec2(ptr));
	}
	else if(type == "size")
	{
		if(ptr = pElem2->Attribute("value"))
			item->setSize( str2ivec2(ptr));
	}
	else if(type == "states")
	{
		int buffer;
		if(pElem2->QueryIntAttribute("visible",&buffer))
			item->setVisible(buffer);
		if(pElem2->QueryIntAttribute("movable",&buffer))
			item->setMovable(buffer);
		if(pElem2->QueryIntAttribute("outside",&buffer))
			item->setOutside(buffer);
	}
	else if(type == "color")
	{
		if(ptr = pElem2->Attribute("value"))
			item->setColor( str2vec4(ptr) );
	}
	else if(type == "texture")
	{
		int buffer;
		pElem2->QueryIntAttribute("id",&buffer);
		//switch la valeur de l'id xml avec l'id reel des textures opengls
		item->setTexture( xml2gl(buffer,idtextab) ); 

	}
	else if(type == "text")
	{
		if(ptr = pElem2->Attribute("value"))
			item->setText( ptr );

		if(ptr = pElem2->Attribute("position"))
			item->setPosText( str2ivec2(ptr) );

		if(ptr = pElem2->Attribute("color"))
			item->setColorText(str2vec4(ptr));

	}
	else
		return RX_ERROR; //on a rien trouvé c'est si triste

	return RX_OK; //on a trouvé quelque chose c'est génial
}

void RxGUI::ItemReader(RxItem2D *item,TiXmlElement *pElem)
{
	TiXmlElement * pElem2 = pElem->FirstChildElement();
	for( pElem2; pElem2; pElem2=pElem2->NextSiblingElement()) 
	{
		
		//lecture des valeurs communs à tous les items
		GlobalItemValues(item,pElem2);
	}
}
void RxGUI::ButtonReader(RxButton *item,TiXmlElement *pElem)
{
	const char *ptr;
	//relève données de l'ojet
	TiXmlElement * pElem2 = pElem->FirstChildElement();
	for( pElem2; pElem2; pElem2=pElem2->NextSiblingElement()) 
	{
		std::string type = pElem2->Value();
		//lecture des valeurs communs à tous les items
		if(GlobalItemValues(item,pElem2))
		{
			continue;
		}
		else if(type == "mousetexture")
		{
			int out = 0, on = 0, ondown = 0;
		    pElem2->QueryIntAttribute("out",&out);
			pElem2->QueryIntAttribute("onup",&on);
			pElem2->QueryIntAttribute("ondown",&ondown);

			item->setIDsTex(xml2gl(out,idtextab),xml2gl(on,idtextab),xml2gl(ondown,idtextab));
		}
		else if(type == "mousecolor")
		{
			glm::vec4 out, on, ondown;
			out = on = ondown = glm::vec4(-1.0f);

			if( ptr=pElem2->Attribute("out") )
				out = str2vec4(ptr);

			if( ptr=pElem2->Attribute("onup") )
				on = str2vec4(ptr);

			if( ptr=pElem2->Attribute("ondown") )
				ondown = str2vec4(ptr);

			item->setColors(out,on,ondown);
		}
		else if(type == "mousetext")
		{
			std::string out, on, ondown;
			if( ptr=pElem2->Attribute("out") )
				out = ptr;

			if( ptr=pElem2->Attribute("onup") )
				on = ptr;

			if( ptr=pElem2->Attribute("ondown") )
				ondown = ptr;
			
			item->setTexts(out.c_str(),on.c_str(),ondown.c_str());
		}
	}
}
void RxGUI::TextBoxReader(RxTextBox *item,TiXmlElement *pElem)
{
	//relève données de l'ojet
	TiXmlElement * pElem2 = pElem->FirstChildElement();
	for( pElem2; pElem2; pElem2=pElem2->NextSiblingElement()) 
	{
		std::string type = pElem2->Value();
		//lecture des valeurs communs à tous les items
		if(GlobalItemValues(item,pElem2))
		{
			continue;
		}
		else if(type == "scroll")
		{
			float speed = 5.0f;
		    pElem2->QueryFloatAttribute("speed",&speed);
			item->setSpeedScroll(speed);
		}
		else if(type == "border")
		{
			int border = 5;
			pElem2->QueryIntAttribute("border",&border);
			item->setBorder(border);
			int test = 1;
		}
		else if(type == "boxtext")
		{
			 item->setText(pElem2->GetText());
		}
		else if(type == "buttonup")
		{
			ButtonReader(&(item->BUp),pElem2);
		}
		else if(type == "buttondown")
		{
			ButtonReader(&(item->BDown),pElem2);
		}
		else if(type == "buttonscroll")
		{
			ButtonReader(&(item->BScroll),pElem2);
		}

	}//fin for
}

