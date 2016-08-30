//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#include "RxHeightMap.h"



RxHeightMap::RxHeightMap(void):RxMesh()
{
	h = w  = 0;
	valide = 0;
	randomTexturing = 0;
	vertFile = RX_HEIGHTMAP_VERT_FILE;
	fragFile = RX_HEIGHTMAP_FRAG_FILE;
	type = RX_HEIGHTMAP_ITEM3D;
	ratio_wh = 4.0f/3.0f;
	fovy = 70;
	step_tr = 0;

}

RxHeightMap::~RxHeightMap(void)
{
	//inutile de detruire vbo, deja supprimé par ~RxMesh()

}
bool RxHeightMap::Load(const char *filename, glm::vec3 _pos, glm::vec3 _scale)
{
	if(filename == NULL) return 0;
	pos = _pos;
	scale = _scale;
	valide = HeightMapFile(filename);

	if(!valide) return 0;
	    ComputeNormals();
		ComputeTriangles();
		ComputeVBO();
		//charge le shader et création des variables uniformes en fonction du nombre de texture
		ComputeShader();
		setDrawOptions();


	return valide;
}

bool RxHeightMap::CreatePerlinMap(glm::vec3 _pos, glm::vec3 _scale, int _w, int _h, float hmin, float hmax, Rx2DPerlinNoise perlinNoise, int smooth)
{
	pos = _pos;
	scale = _scale;
	h = _h;
	w = _w;
	hmap.clear();
	hmap.reserve(w*h);
	valide = HeightMapPerlin(hmin,hmax,perlinNoise );

	if(!valide) return 0;
	if(smooth>0)
		SmoothFunction(smooth);

	ComputeNormals();
	ComputeTriangles();
	ComputeVBO();
	//charge le shader et création des variables uniformes en fonction du nombre de texture
	ComputeShader();
    setDrawOptions();
	return valide;
}
bool RxHeightMap::CreateRandomMap(glm::vec3 _pos, glm::vec3 _scale, int _w, int _h, float hmin, float hmax, int smooth)
{
	pos = _pos;
	scale = _scale;
	h = _h;
	w = _w;
	hmap.clear();
	hmap.reserve(w*h);
	HeightMapRand(hmin,hmax);
	
	
	if(smooth>0)
		SmoothFunction(smooth);

	ComputeNormals();
	ComputeTriangles();
	ComputeVBO();
	//charge le shader et création des variables uniformes en fonction du nombre de texture
	ComputeShader();
    setDrawOptions();
	return valide;
}


bool RxHeightMap::HeightMapFile(const char * filename)
{
	
	//on charge notre height map à partir de l'image cible
	SDL_Surface * surface = IMG_Load(filename);
    //verifie qu'il n'y a pas d'erreur de chargement de l'image
    if(surface == NULL)
	{
		RXLOGGERERROR("[RxHeightMap][HeightMapFile] Load fail : "<<filename);
		return RX_ERROR;
	}
	
	//relève les dimensions de l'image
	h = surface->h;
	w = surface->w;
	hmap.clear();
	hmap.reserve(w*h); //reserve l'espace necessaire en fonction de la dimension de l'image
	
	//on relève la hauteur des noeuds
	
	//cas d'une image en niveau de gris
	unsigned int i,j;
	Uint8 r,g,b;

	RxNodeMap node;
	
	//calcul des vertices en fonction de la couleur des pixels et des paramétres d'entrées
	
	for(j = 0; j < h; j++)
	{
		for(i = 0; i < w; i++)
		{
		
			SDL_GetRGB(RxGetSDLPixel(surface,i,j), surface->format, &r, &g, &b); //relève la couleur du pixel (i,j)
			
			//enregistre le vertex remis à l'échelle
			node.setPosition(glm::vec3((float)i ,(float)r ,(float)j ));
			
			//par défaut on définit les coordonnées textures
			//[A MODIFIER]
			node.texCoord.x = (float)i;
			node.texCoord.y = (float)j;

			//génére une id aléatoirement
			if(randomTexturing) 
				node.idTex = GenCoordTexHeight(r,255); 

			//ajoute le noeud à la liste
			hmap.push_back(node);
		}
	}
	
	SDL_FreeSurface(surface); //libération de la surface

	RXLOGGER("[RxHeightMap][HeightMapFile] File OK : "<<filename);
	return RX_OK;
}

void RxHeightMap::HeightMapRand(float hmin, float hmax)
{
	RxNodeMap node;
	unsigned int i,j,r;

	//calcul des vertices en fonction de la couleur des pixels et des paramétres d'entrées
	
	for(j = 0; j < h; j++)
	{
		for(i = 0; i < w; i++)
		{
			//attention pr physx les heightmap sont définit pas des niveaux d'entier
			r = (unsigned int) ceil(random(hmin,hmax));
			//enregistre le vertex remis à l'échelle
			node.setPosition(glm::vec3((float)i,(float)r ,(float)j));
			
			//par défaut on définit les coordonnées textures
			//[A MODIFIER]
			node.texCoord.x = (float)i;
			node.texCoord.y = (float)j;

			//génére une id aléatoirement
			if(randomTexturing) 
				node.idTex =  GenCoordTexHeight((float)r,hmax);

			//ajoute le noeud à la liste
			hmap.push_back(node);
		}
	}
	
}
bool RxHeightMap::HeightMapPerlin(float hmin, float hmax,  Rx2DPerlinNoise perlinNoise)
{

	noise::module::Perlin perlinModule;
	perlinModule.SetOctaveCount (perlinNoise.octaveCount);
    perlinModule.SetFrequency (perlinNoise.frequency);
    perlinModule.SetPersistence (perlinNoise.persistence);
	perlinModule.SetSeed(perlinNoise.seed);

    noise::utils::NoiseMap heightMap;
	noise::utils::NoiseMapBuilderPlane textureBuilder;
	textureBuilder.SetSourceModule (perlinModule);
	textureBuilder.SetDestNoiseMap (heightMap);
	textureBuilder.SetDestSize (w, h);
	textureBuilder.SetBounds (perlinNoise.leftLowerCorner.x,perlinNoise.rightUpperCorner.x,perlinNoise.leftLowerCorner.y,perlinNoise.rightUpperCorner.y);
	textureBuilder.Build();
	


	noise::utils::RendererImage renderer;
	noise::utils::Image image;
	renderer.SetSourceNoiseMap (heightMap);
	renderer.SetDestImage (image);
	renderer.ClearGradient ();
	renderer.AddGradientPoint (-1.0000, utils::Color (  0,   0, 128, 255)); // deeps
	renderer.AddGradientPoint (-0.2500, utils::Color (  0,   0, 255, 255)); // shallow
	renderer.AddGradientPoint ( 0.0000, utils::Color (  0, 128, 255, 255)); // shore
	renderer.AddGradientPoint ( 0.0625, utils::Color (240, 240,  64, 255)); // sand
	renderer.AddGradientPoint ( 0.1250, utils::Color ( 32, 160,   0, 255)); // grass
	renderer.AddGradientPoint ( 0.3750, utils::Color (224, 224,   0, 255)); // dirt
	renderer.AddGradientPoint ( 0.7500, utils::Color (128, 128, 128, 255)); // rock
	renderer.AddGradientPoint ( 1.0000, utils::Color (255, 255, 255, 255)); // snow
	renderer.Render();	


	RxNodeMap node;
	unsigned int i,j;
	//calcul des vertices en fonction de la couleur des pixels et des paramétres d'entrées
	for(j = 0; j < h; j++)
	{
		for(i = 0; i < w; i++)
		{
			//attention pr physx les heightmap sont définit pas des niveaux d'entier
			//r = (unsigned int) ceil(PerlinMap.getValue2DMap(i,j)*(hmax-hmin)+hmin);

			float rf = (heightMap.GetValue(i,j)+1.0f)/2.0f;
			rf = rf*(hmax-hmin)+hmin;

			//enregistre le vertex remis à l'échelle
			node.setPosition(glm::vec3((float)i ,(float) rf ,(float)j ));
			
			//par défaut on définit les coordonnées textures
			//[A MODIFIER]
			node.texCoord.x = (float)i;
			node.texCoord.y = (float)j;

			noise::utils::Color ucolor = image.GetValue(i,j);

			node.color = glm::vec3(ucolor.red/256.0f,ucolor.green/256.0f,ucolor.blue/256.0f);

			//génére une id aléatoirement
			if(randomTexturing) 
				node.idTex = GenCoordTexHeight((float)rf,hmax);

			//ajoute le noeud à la liste
			hmap.push_back(node);
		}
	}
	
	return 1;
}



void RxHeightMap::SmoothFunction(int n)
{	

    std::vector<float> tmp_hmap;
	tmp_hmap.resize(w*h);
	unsigned int i,j,m,l,k;
	float a;
	//calcul des vertices en fonction de la couleur des pixels et des paramétres d'entrées
	for(j = 0; j <  h; j++)
	{
		for(i = 0; i <  w; i++)
		{
					a=0;
					m=0;
					for (k=j-n; k<=j+n; k++)
					{
						for (l=i-n; l<=i+n; l++)
						{
							if ((k>=0) && (k<h) && (l>=0) && (l<w))
							{
								m++;
								a+=hmap[Find_i(l,k)].height;
							}
						}
					}
					tmp_hmap[Find_i(i,j)] = (float)a/m;
		}
	}
	for(i = 0; i < w*h; i++)
	{
		//attention pr physx les heightmap sont définit par des niveaux d'entier
		hmap[i].setH ( tmp_hmap[i]);
	}

}

void RxHeightMap::ComputeNormals()
{
		//calcul des normals
	glm::vec3 v[3];
	glm::vec3 n[4];
	unsigned int i,j;
	for(j = 0; j < h; j++)
	{
		for(i =0; i < w; i++)
		{
			/*********************************


                 (i,j+1)B
			-----------------
			|	   /| \		|
			|	/	|   \	|
			| /(i,j)|O	  \ |A
    (i-1,j)C----------------- (i+1,j)A
			| \		|      /|
			|   \	|	/	|
			|	  \	|/		|
			-----------------
				(i,j-1)D
			
			on calcul les normals des triangles rectangles en O à partire des point A,B,C, et D
			la normale au point O est égale à la moyen des normal des triangles rectangles


			***********************************/
			v[0] = hmap[Find_i(i,j)].pos;
			v[1] = hmap[Find_i(i+1,j)].pos;
			v[2] = hmap[Find_i(i,j+1)].pos;
			n[0] = v[1]-v[0]+ v[2]-v[0];
			n[0]= glm::normalize(n[0]);

			v[1] = hmap[Find_i(i,j+1)].pos;
			v[2] = hmap[Find_i(i-1,j)].pos;
			n[1] = v[1]-v[0]+ v[2]-v[0];
			n[1]= glm::normalize(n[1]);
			
			v[1] = hmap[Find_i(i-1,j)].pos;
			v[2] = hmap[Find_i(i,j-1)].pos;
			n[2] = v[1]-v[0]+ v[2]-v[0];
			n[2]= glm::normalize(n[2]);
			
			v[1] = hmap[Find_i(i,j-1)].pos;
			v[2] = hmap[Find_i(i+1,j)].pos;
			n[3] = v[1]-v[0]+ v[2]-v[0];
			n[3]= glm::normalize(n[3]);

			v[0] = hmap[Find_i(i,j)].pos;
			v[1] = hmap[Find_i(i+1,j)].pos;
			v[2] = hmap[Find_i(i+1,j+1)].pos;
			n[0] = v[1]-v[0]+ v[2]-v[0];
			n[0]= glm::normalize(n[4]);;

			v[1] = hmap[Find_i(i+1,j+1)].pos;
			v[2] = hmap[Find_i(i,j+1)].pos;
			n[1] = v[1]-v[0]+ v[2]-v[0];
			n[1]= glm::normalize(n[5]);


			n[0] = n[0]+n[1]+n[2]+n[3];
			n[0]= glm::normalize(n[0]);
			hmap[Find_i(i,j)].normal = n[0];

		}
	}

}

void RxHeightMap::ComputeTriangles()
{
	tSubMeshes.resize(1);
	std::vector<RxFace> &tmap = tSubMeshes[0].tFace;
	tmap.clear();
	tmap.reserve(w*h*2);
	unsigned int i,j;
	RxFace t;
	
	for(j = 0; j < h-1; j++)
	{
		for(i = 0; i < w-1; i++)
		{
				/*  unité terrain
			  (i,j+1)	 (i+1,j+1)
					 ---
					| 2/|
					| /1|
					 ---
				(i,j)    (i+1,j)
				*/
			/*	//triangle n°1 d'une unité terrain
			    t.set( Find_i(i,j), Find_i(i+1,j), Find_i(i+1,j+1));
				tmap.push_back(t);
				
				//triangle n°2 d'une unité terrain
				t.set( Find_i(i,j), Find_i(i+1,j+1), Find_i(i,j+1));
				tmap.push_back(t);*/

				//correspond à PhysX
				//triangle n°1 d'une unité terrain
			    t.set( Find_i(i,j), Find_i(i,j+1), Find_i(i+1,j+1));
				tmap.push_back(t);
				
				//triangle n°2 d'une unité terrain
				t.set( Find_i(i+1,j), Find_i(i,j), Find_i(i+1,j+1));
				tmap.push_back(t);
		}
	}
	
}
glm::ivec3 RxHeightMap::getNearbyFaces(unsigned int k)
{

	/*		  (i,j+1)	 (i+1,j+1)
					 ---
					| \1|
					| 0\|
					 ---
				(i,j)    (i+1,j)
*/
	unsigned wf = (w-1)*2; //nombre de triangle selon la largeur
	unsigned hf = (h-1)*2; //nombre de triangle selon la longeur
	
	unsigned int oddnumber = (k%2); // paire ou impaire ?
	
	glm::ivec3 nearby;
	//détermination des voisins de la faces d'indices k
	//voisin de diagonale

	if( oddnumber ) // face impaire
	{
		nearby.x = k-1;  //voisin de diagonale
		
		//voisin de droite si non sur les bords
		if( (k+1)%wf ) nearby.y = k+1; 
		else nearby.y = -1; //on est sur le bord inférieur pas de voisin du dessous
		
		//voisin du haut
		nearby.z = k+wf;
		if( (unsigned int)nearby.z  >= h*w ) nearby.z = -1;
	}
	else //face paire
	{
		//voisin de diagonale
		nearby.x = k+1; 

		//voisin de gauche si non sur les bords
		if( k%wf ) nearby.y = k-1; 
		else nearby.y = -1; //on est sur le bord inférieur pas de voisin du dessous

		//voisin du bas
		nearby.z = k - wf; //décalage de wf triangle
	}
	
	return nearby;
}

void RxHeightMap::ComputeVBO()
{

	std::vector<RxFace> &tmap = tSubMeshes[0].tFace;	

    std::vector<glm::vec3>	tPosition;
	std::vector<glm::vec3>	tNormal;
	std::vector<glm::vec2>	tTexcoord;
	std::vector<glm::vec3>	tColor;
	//ici on se sert de tTexMelange pour transmettre au shader les indices textures à utiliser et mélanger
	
	std::vector<glm::ivec3>	tTexMelange;

	std::vector<glm::vec3>	tMixing;

	tPosition.reserve(tmap.size()*3);
	tNormal.reserve(tmap.size()*3);
	tTexcoord.reserve(tmap.size()*3);
	tColor.reserve(tmap.size()*3);

	tTexMelange.reserve(tmap.size()*3);
	tMixing.reserve(tmap.size()*3);
    unsigned int i,nb_triangle;
			nb_triangle = tmap.size();
            for(i = 0; i < nb_triangle; i++)
            {
                //premier triangle du carré	
				glm::ivec3 MelangeIDtex(	(float)hmap[tmap[i].id[0]].idTex,
									(float)hmap[tmap[i].id[1]].idTex,
									(float)hmap[tmap[i].id[2]].idTex );	

				/*glm::vec3 TangentIDtex(	0,
									1,
									2);*/

                    tTexcoord.push_back(hmap[tmap[i].id[0]].texCoord);
                    tNormal.push_back(hmap[tmap[i].id[0]].normal);
                    tPosition.push_back(hmap[tmap[i].id[0]].pos);
					tColor.push_back(hmap[tmap[i].id[0]].color);
					tMixing.push_back(glm::vec3(1.0f,0.0f,0.0f));
					tTexMelange.push_back(MelangeIDtex);

                    tTexcoord.push_back(hmap[tmap[i].id[1]].texCoord);
                    tNormal.push_back(hmap[tmap[i].id[1]].normal);
                    tPosition.push_back(hmap[tmap[i].id[1]].pos);
					tColor.push_back(hmap[tmap[i].id[1]].color);
					tMixing.push_back(glm::vec3(0.0f,1.0f,0.0f));
					tTexMelange.push_back(MelangeIDtex);

                    tTexcoord.push_back(hmap[tmap[i].id[2]].texCoord);
                    tNormal.push_back(hmap[tmap[i].id[2]].normal);
                    tPosition.push_back(hmap[tmap[i].id[2]].pos);
					tColor.push_back(hmap[tmap[i].id[2]].color);
					tMixing.push_back(glm::vec3(0.0f,0.0f,1.0f));
					tTexMelange.push_back(MelangeIDtex);
					
            }

	vbo.AddArrayBufferData(RX_VERTEX_ATTRIB_INDEX,tPosition);
	vbo.AddArrayBufferData(RX_COORD0_ATTRIB_INDEX,tTexcoord);
	vbo.AddArrayBufferData(RX_NORMAL_ATTRIB_INDEX,tNormal);
	vbo.AddArrayBufferData(RX_COLOR_ATTRIB_INDEX,tColor);
	vbo.AddArrayBufferData(RX_DATA1_ATTRIB_INDEX,tMixing);
	vbo.AddArrayBufferData(RX_DATA0_ATTRIB_INDEX,tTexMelange);


	if(!vbo.Create(GL_STATIC_DRAW))
		RXLOGGERERROR("[RxHeightMap][ComputeVBO] VBO Fail");
	

	nbFaces = getNbFaces();



}
void RxHeightMap::ComputeShader()
{
	    

		//charge le shader et création des variables uniformes en fonction du nombre de texture
		shadermap.Load(vertFile.data(), fragFile.data());




		geometryshader.Load(RX_GEOMETRY_VERT_FILE,RX_GEOMETRY_FRAG_FILE);
}
void RxHeightMap::DrawGeometryOnly()
{
	if(!valide) return;

	geometryshader.Enable();
	geometryshader.SendMat4f("projection",0,(*projection));
	geometryshader.SendMat4f("modelview",0,(*modelview)*glm::translate(glm::mat4(),pos)*glm::scale(glm::mat4(),scale));
	vbo.Enable();
        
	unsigned int nb_triangle = tSubMeshes[0].tFace.size();

		//on dessine l'ensemble des vertices, on a 3 fois plus de vertices que de triangles
		
			//glTranslatef(pos.x,pos.y,pos.z);
			glDrawArrays(GL_TRIANGLES, 0, nb_triangle*3);
		

	vbo.Disable();
	geometryshader.Disable();

}

void RxHeightMap::DrawGeometryOnly(glm::vec3 cpos, glm::vec3 cdir)
{
	if(!valide) return;

	geometryshader.Enable();
	geometryshader.SendMat4f("projection",0,(*projection));
	geometryshader.SendMat4f("modelview",0,(*modelview)*glm::translate(glm::mat4(),pos)*glm::scale(glm::mat4(),scale));

	vbo.Enable();
        //on dessine nos sections de RxTunnel visible
		
		unsigned int i;
		for(i = 0; i < sections.size();i++)
				glDrawArrays(GL_TRIANGLES, sections[i].x*3, (sections[i].y- sections[i].x)*3 );

	vbo.Disable();
	geometryshader.Disable();
}
void RxHeightMap::Draw()
{
	DrawAll();
}
void RxHeightMap::Draw(glm::vec3 cpos, glm::vec3 cdir)
{
		//Calcul des sections de terrain visible
		ComputeFrustrumOptimisation(cpos, cdir);

		DrawAll(cpos,cdir);
			//DrawAll();
}

void RxHeightMap::DrawAll()
{

	if(!valide) return;
	
	shadermap.Enable();
	//passe les textures utilisées aux shaders
	//RXLOGGER<<"APPLICATION SHADER"<<endl;
	//glActiveTexture (GL_TEXTURE0);
	shadermap.Send1i("arrayMap",0);
	shadermap.SendMat4f("projection",0,(*projection));
	shadermap.SendMat4f("modelview",0,(*modelview)*glm::translate(glm::mat4(),pos)*glm::scale(glm::mat4(),scale));	
	material.Bind();


	vbo.Enable();
		//on dessine l'ensemble des vertices, on a 3 fois plus de vertices que de triangles
	unsigned int nb_triangle = tSubMeshes[0].tFace.size();	
	glDrawArrays(GL_TRIANGLES, 0, nb_triangle*3);

	vbo.Disable();
	shadermap.Disable();
}

void RxHeightMap::DrawAll(glm::vec3 cpos, glm::vec3 cdir)
{
	if(!valide) return;
	
	shadermap.Enable();
	//passe les textures utilisées aux shaders
	//RXLOGGER<<"APPLICATION SHADER"<<endl;
	glActiveTexture (GL_TEXTURE0);
	shadermap.Send1i("arrayMap",0);
	shadermap.SendMat4f("projection",0,(*projection));
	shadermap.SendMat4f("modelview",0,(*modelview)*glm::translate(glm::mat4(),pos)*glm::scale(glm::mat4(),scale)*glm::scale(glm::mat4(),scale));	
	material.Bind();

	vbo.Enable();
        //on dessine nos sections visibles
		unsigned int i;
		for(i = 0; i < sections.size();i++)
				glDrawArrays(GL_TRIANGLES, sections[i].x*3, (sections[i].y- sections[i].x)*3 );
	vbo.Disable();
	shadermap.Disable();
}

unsigned int RxHeightMap::Find_i(unsigned int x, unsigned int z)
{
	//evite les debordements
	if( x > w-1) x = w-1;
	if( z > h-1) z = h-1;
	if( x < 0) x = 0;
	if( z < 0) z = 0;

	return (z*w+x);
}


glm::ivec2 RxHeightMap::Find_xz(unsigned int i)
{
	if( i >= w*h ) return glm::ivec2(0,0);
	unsigned int _x, _z;
	
	_z = i/w;
	_x = i - w*_z;
	if( _x*_z >= w*h ) return glm::ivec2(0,0);

	return glm::ivec2(_x,_z);

}

void RxHeightMap::EnableRandomTexturing()
{ 
		randomTexturing = true;
}


GLuint RxHeightMap::RandTex(unsigned int max)
{
	if(!max) max = material.colorMap.n;
	GLuint val;
	static bool init_rand = 0;
//	static int last_rand = 1;
	if(!init_rand) 
	{
		srand ( SDL_GetTicks() );
		init_rand=1;
	}

   /* generate random number: */
	val = rand() % (max);
	/*if(  (rand() % 10) < 8)
	val = last_rand;
	last_rand = val;*/

	return val;
}
GLuint RxHeightMap::GenCoordTexHeight(float height, float max)
{
	unsigned int n_t = 4;
	n_t = material.colorMap.n;

	static float max2 = 1;
	if(height > max2) max2 = height;

	float step = max2/n_t;
	unsigned int val = (unsigned int)(height/step);

	if(val >= n_t) val = n_t-1;

	return val;
}


bool RxHeightMap::SubdivisionMapQTree(void *data, void **data0, void **data1, void **data2, void **data3)
{

	glm::vec2 cpos; //position de la camera
	glm::vec2 cdir; //direction de la camera

	DQuad &quad = *((DQuad*) data); //relève le quad
	
	glm::ivec2 sizeQuad = quad.end - quad.begin;//dimension du quad 
	if(sizeQuad.x < minQuad || sizeQuad.y < minQuad) //quad trop petit on ne divise plus
		return 0;

	//passage des coordonnées discrets i,j au coordonnées réels.
	glm::vec2 rbegin, rend, rsize;
	rbegin = glm::vec2(hmap[Find_i(quad.begin.x,quad.begin.y)].pos.x,hmap[Find_i(quad.begin.x,quad.begin.y)].pos.z);
	rend = glm::vec2(hmap[Find_i(quad.end.x,quad.end.y)].pos.x,hmap[Find_i(quad.end.x,quad.end.y)].pos.z);
	rsize = rend-rbegin;

	//vecteur entre les sommets du quad et la camera
	glm::vec2 vA,vB,vC,vD; 
	vA = rbegin - cpos;
	vC = rend - cpos;
	vB = glm::vec2(rend.x,rbegin.y) - cpos;
	vD = glm::vec2(rbegin.x,rend.y) - cpos;

	//calcul le cosinus de l'angle d'ouverture de la camera selon l'axe x.
	float fovx = getfovx(fovy,ratio_wh); 

	//calcul des normales au plan de clipping latérale
	glm::vec2 n1,n2;
	n1 = cdir;
	n2 = cdir;

	n1 = glm::vec2(glm::rotate(glm::mat4(), 90+fovx, glm::vec3(0.0f,0.0f,1.0f))*glm::vec4(n1,0.0f,0.0f));
	n2 = glm::vec2(glm::rotate(glm::mat4(), 90-fovx, glm::vec3(0.0f,0.0f,1.0f))*glm::vec4(n2,0.0f,0.0f));
	//n1.rotate(90+fovx);
	//n2.rotate(90-fovx);
	return 1;
}

void RxHeightMap::setDrawOptions(float _fovy, float _ratio, unsigned int _step)
{
		
		step_tr = _step; //par défaut 1/10ème du min(w,h).
		if(!step_tr)
			step_tr = (int)(0.10f*glm::min(w,h)+1);
		
		ratio_wh = _ratio; 
		fovy = _fovy; 
}


void RxHeightMap::ComputeFrustrumOptimisation(glm::vec3 cpos, glm::vec3 cdir)
{
	
	//ne pas oublier de prendre en compte la position de la map;
	cpos -= pos;

	static glm::vec3 last_cpos = cpos*2.0f;
	static glm::vec3 last_cdir = cdir*2.0f;

	if(!valide) return;

	if( (last_cpos == cpos) && (last_cdir == cdir)) //rien n'a changé pas besoin de tout recalculer
		return;

	last_cpos = cpos;
	last_cdir = cdir;

	//enregistre un couple d'indices (x,z) des vertexs visibles
	sections.clear();
	sections.reserve(glm::max(w,h));	
    std::vector<RxFace> &tmap = tSubMeshes[0].tFace;
    //les parties visibles à l'écran sont des sections de terrain allant par exemple
	// du point visible n au point visible n+12 que l'on sauvegarde dans sections
	//pour détecter les debuts et fin de sections on regarde les fronts montant et front descendant (correspondant
	//au n pr le front montant, et au n+12 au front descendant de l'exemple)
	

	//on aura un front montant quand on aura (0,1) et un front descendant pour (1,0);
	bool nv = 0; 
	bool last_nv = 0;
	int tmp_i = 0;

	cdir = glm::normalize(cdir);
	
	
	int i;

	int nb_triangle = tSubMeshes[0].tFace.size();
	
	cpos -= cdir*((float)step_tr); //creation d'un offset sur la position de la camera pour eviter un effet de bord
	


	//calcul le cosinus de l'angle d'ouverture de la camera selon l'axe x.
	float cosfovx = cos(getfovx(fovy,ratio_wh)/2); 
   // float cosfovx = 0.708f;


    for(i = 0; i < nb_triangle; i+=step_tr)
	{
				/*  unité terrain
			  (i,j+1)	 (i+1,j+1)
					 ---
					| 2/|
					| /1|
					 ---
				(i,j)    (i+1,j)
				*/
				//section n°1 d'une unité terrain (composé de deux triangle
				//vec pos_sect(i,hmap[Find_i(i,j],j); 
			    //t.set( Find_i(i,j), Find_i(i+1,j+1), Find_i(i+1,j));
				//triangle n°2 d'une unité terrain
				//t.set( Find_i(i,j), Find_i(i+1,j+1), Find_i(i,j+1));

						
				//on determine si l'objet est visible
				//position du vertex
				glm::vec3 vpos =  glm::vec3(hmap[tmap[i].id[0]].pos.x * scale.x, hmap[tmap[i].id[0]].pos.y * scale.y,hmap[tmap[i].id[0]].pos.z * scale.z);
				//direction vertex-camera
				glm::vec3 cp = (vpos - cpos);
				cp = glm::normalize(cp);

		
				nv =( (glm::dot(cp,cdir)) >= cosfovx) ;
	
				//front montant, debut de la section visible
				if(nv && !last_nv)
				{
					//on enregistre la possition du front montant
					//-1 c'est pour prendre en compte le segement d'avant histoire de bien tout dessiner à l'ecran
					tmp_i= (i-step_tr); //attention au cas ou i = 0 de pas se retrouver avec un indice à -1
					if(tmp_i<0) tmp_i = 0;
				}
				//front descendant, fin de la section visible, on l'enregistre
				//dans le cas ou on arrive au but on est obligé de fermer la section car l'objet d'après est forcement
				//invisible vu qu'il n'existe pas
				else if(last_nv && (!nv || (i+step_tr >= nb_triangle)))
				{ 
						//on prend le point suivant histoire d'etre sur de bien tout afficher à l'écran
						//il n'y a pas de debordement d'indice ici, on a un segement de plus qui boucle la spline
					int tmp_i2 = (i+step_tr);
					if( tmp_i2 >= nb_triangle) tmp_i2 = nb_triangle-1;

					sections.push_back(glm::ivec2(tmp_i,tmp_i2));
				}

				last_nv = nv;
		
	}


	
}

