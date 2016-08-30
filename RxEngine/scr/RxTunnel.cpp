//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#


#include "RxTunnel.h"

#ifdef _DEBUG
//#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


//b-spline
mat4 m_bspline( -0.1666666F, 0.5F,      -0.5F,      0.1666666F,
                0.5F      ,-1.0F,       0.5F,      0.0F,
               -0.5F      , 0.0F,       0.5F,      0.0F,
                0.1666666F, 0.6666666F, 0.1666666F,0.0F );

//hermite
mat4 m_hspline( -0.5F, 1.5F,-1.5F, 0.5F,
                1.0F,-2.5F, 2.0F,-0.5F,
               -0.5F, 0.0F, 0.5F, 0.0F,
                0.0F, 1.0F, 0.0F, 0.0F ) ;

RxTunnel::RxTunnel():RxItem3D()
{
  type = RX_TUNNEL_ITEM3D;
  circle = 1;
  disques = NULL;
  n_disques = NULL;
  nb_d = 0;
  radius = 4;
  nb_seg = 16;
}
RxTunnel::~RxTunnel()
{
   ReleaseDisques();
}
void RxTunnel::Lisse(vec3 *p,int n, mat4 m) {
  int i,j,k ;
  float tt[4],ttt[4],x,y,z ;

  for ( i = 1 ; i < n ; i++ )
  {
        float t =(float) i/(n-1) ;
        tt[0] = t*t*t ;
        tt[1] = t*t ;
        tt[2] = t ;
        tt[3] = 1 ;
        for ( j = 0 ; j < 4 ; j++ )
          for ( k = 0,ttt[j] = 0 ; k < 4 ; k++ )
            ttt[j] += tt[k] * m.mat[k*4+j];///m.mat[k+j*4] ///m.mat[k][j] ;
        x = y = z = 0 ;
        for ( j = 0 ; j < 4 ; j++ )
        {
              x += ttt[j] * p[j].x ;
              y += ttt[j] * p[j].y ;
              z += ttt[j] * p[j].z ;
        }
        courbe_lisse.AddPoint(vec3(x,y,z))   ; //sauvegarde des points lissés
   }
}

void RxTunnel::B_spline(courbe *p,mat4 m,int n)
{
  //si la structure contient déjà des informations on les supprimes
  courbe_lisse.Release();

  //on alloue l'espace necessaire
  courbe_lisse.n = (n-1)*(descripteur.n-3);
  courbe_lisse.p.reserve((n-1)*(descripteur.n-3));

  //on crée notre spline
  for ( unsigned int i = 0 ; i < p->n-3 ; i++ )
    Lisse(&p->p[i],n,m) ;


}

void RxTunnel::DrawSplinePoints()
{
    unsigned int i;

    glBegin(GL_POINTS);

        for(i = 0; i < descripteur.n; i++)
          glVertex3fv( descripteur.p[i].v );

    glEnd();
}
void RxTunnel::DrawSpline()
{
    unsigned int n;
    glBegin(GL_LINE_STRIP);

        for(n = 0; n < courbe_lisse.n; n++)
        glVertex3fv( courbe_lisse.p[n].v );

    glEnd();

}
bool RxTunnel::LoadTunnel(vec3 *descripteur_c,int n, int def, float r, int seg)
{
    if(descripteur_c == NULL)
    {
        RXLOGGER<<"[ERROR] Pointeur Null"<<std::endl;
        return 0;
    }
    else if( n < 4)
    {
        RXLOGGER<<"[ERROR] RxTunnel pas assez de points de description"<<std::endl;
        return 0;
    }

    radius = r;
    nb_seg = seg;

    if(circle) n+=3;

    descripteur.Release();
    descripteur.n = n;
    descripteur.p.reserve(n);


    int i;
    for(i = 0; i < n -3*circle ; i++)
        descripteur.AddPoint(descripteur_c[i]);

    //rajoute les 3 points de départ pour boucler la spline
    if(circle)
    {
         descripteur.AddPoint(descripteur.p[0]);
         descripteur.AddPoint(descripteur.p[1]);
         descripteur.AddPoint(descripteur.p[2]);
    }

    B_spline(&descripteur,m_hspline, def);
    CalculTunnel();
    return 1;
}
 void RxTunnel::Log(bool b)
 {
            unsigned int n;
     if(b)
     {
         for(n = 0; n < courbe_lisse.n; n++)
          RXLOGGER <<n<<": "<< courbe_lisse.p[n].v[0] <<" : "<<courbe_lisse.p[n].v[1]<<" : "<<courbe_lisse.p[n].v[2] << std::endl;
     }
     else
     {

         for(n = 0; n < descripteur.n; n++)
          RXLOGGER <<n<<": "<< descripteur.p[n].v[0] <<" : "<<descripteur.p[n].v[1]<<" : "<<descripteur.p[n].v[2] << std::endl;
     }
 }
void RxTunnel::CalculTunnel()
{

    unsigned int n,i;
    ReleaseDisques();
    CreateDisques(courbe_lisse.n);


    //pour l'ensemble des points de la courbe
    for(n = 0; n < courbe_lisse.n; n++)
    {
        vec3 drt;
        //calcul la direction de la trajectoire
        //attention aux cas particulier des points en extremiter
        if( n == 0 && circle)
           drt = (courbe_lisse.p[1]-courbe_lisse.p[courbe_lisse.n-1]);
        else if( n == (courbe_lisse.n-1) && circle)
           drt = (courbe_lisse.p[0]-courbe_lisse.p[n-1]);
        else if(n == 0 && !circle)
           drt = (courbe_lisse.p[1]-courbe_lisse.p[0]);
        else if(n == (courbe_lisse.n-1) && !circle)
           drt = (courbe_lisse.p[n]-courbe_lisse.p[n-1]);
        else
           drt = (courbe_lisse.p[n+1]-courbe_lisse.p[n-1]);

        drt.normalize();

        //calcul la normale à la trajectoir
        vec3 n2drt(-drt.z, 0, drt.x);
        n2drt.normalize();


        mat3 rot;
        float alpha = 0; //en degree


        //creation du RxTunnel en dessinant des cercles autours des points lissés
        for(i = 0; i < nb_seg; i++)
        {
            rot.rotate(drt,alpha); //calcul la matrice de rotation en fonction de alpha

            vec3 tmp = rot*n2drt; //on tourne le vecteur d'un angle alpha

            //on calcul la position du point par rapport à la spline
            disques[n].AddPoint(courbe_lisse.p[n] + tmp*radius);

            //on a directement la normale de ce point qui est egale à tmp
            tmp.normalize() ;
            n_disques[n].AddPoint( tmp );

            alpha+= 360/nb_seg;
        }

		rot.rotate(drt,0.0f);
		vec3 tmp = rot*n2drt;
        disques[n].AddPoint(courbe_lisse.p[n] + tmp*radius);
        tmp.normalize() ;
        n_disques[n].AddPoint( tmp );


        //
    }

	//creation des VBO associés
    ComputeVBO();
   
}
void RxTunnel::Draw(vec3 pos, vec3 dir)
{

	unsigned int i;
	vector<ivec2> sections;
	//on reserve le maximum de section possible pour ne pas perdre du temps à chacun
	//allocation de mémoire pour une nouvelle section.
	//Pour une section il faut un debut et une fin soit au minimum 2 points
	//d'ou n/2 maximum de nombre de section. Le +1 c'est histoire de pas prendre un 0 dans le cas ou n < 2;
	sections.reserve(courbe_lisse.n/2+1);
	

	//les parties visibles à l'écran sont des sections du RxTunnel allant par exemple
	// du point visible n au point visible n+12 que l'on sauvegarde dans sections
	//pour détecter les debuts et fin de sections on regarde les fronts montant et front descendant (correspondant
	//au n pr le front montant, et au n+12 au front descendant de l'exemple)

	//on aura un front montant quand on aura (0,1) et un front descendant pour (1,0);
	bool nv = 0; 
	bool last_nv = 0;
	unsigned int tmp_i = 0;

	dir.normalize(); //utile dans le cas 40°

	for(i = 0; i < courbe_lisse.n; i++)
	{
		
		
		//on determine si l'objet est visible
		vec3 cp = courbe_lisse.p[i]-pos;

		//dans le cas d'un angle de vision de 90° par rapport à la direction de la camera
		//nv = ((cp*d) >= 0.0f); //si le produit scalaire est positif alors le point est visible depuis la camera
		
		//dans le cas d'un angle à 45°
		cp.normalize();
		//produit scalaire correspond au cos de (cp,d), pour 45° cos(45°) = 0.7071067811
		//cos(45°/2) = 0,92387953
		nv = ((cp*dir) >= 0.7507953f);   


		//front montant, debut de la section visible
		if(nv && !last_nv)
		{
			//on enregistre la possition du front montant
			//-1 c'est pour prendre en compte le segement d'avant histoire de bien tout dessiner à l'ecran
			tmp_i= (i-1) * (i > 0); //attention au cas ou i = 0 de pas se retrouver avec un indice à -1
		}
		//front descendant, fin de la section visible, on l'enregistre
		//dans le cas ou n arrive en but on est obligé de fermer la section car l'objet d'après est forcement
		//invisible vu qu'il n'existe pas
		else if(last_nv && (!nv || i==courbe_lisse.n-1))
		{ 
			    //on prend le point suivant histoire d'etre sur de bien tout afficher à l'écran
			    //il n'y a pas de debordement d'indice ici, on a un segement de plus qui boucle la spline
					sections.push_back(ivec2(tmp_i,i+1));
		}

		last_nv = nv;
	}




	tunnel_VBO.Enable();
        //on dessine nos sections de RxTunnel visible
		for(i = 0; i < sections.size();i++)
				glDrawElements(GL_TRIANGLES,(sections[i].y- sections[i].x)*nb_seg*6,GL_UNSIGNED_INT,tFaces[sections[i].x*nb_seg*2].id);
		

	tunnel_VBO.Disable();

}


void RxTunnel::DrawGLFunction()
{
    unsigned int n,i;
glBegin(GL_TRIANGLES);
  for(n = 0; n < nb_d-1; n++)
    {

            for(i = 0; i < nb_seg-1; i++)
            {

                glTexCoord2f(0.0f, i/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n ].p[i]);
                    glVertex3fv( disques[ n ].p[i]);

                glTexCoord2f(1.0f, i/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n+1 ].p[i]);
                    glVertex3fv( disques[ n+1 ].p[i]);

                glTexCoord2f(0.0f, (i+1)/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n ].p[i+1]);
                    glVertex3fv( disques[ n ].p[i+1]);

                glTexCoord2f(0.0f, (i+1)/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n ].p[i+1]);
                    glVertex3fv( disques[ n ].p[i+1]);

                glTexCoord2f(1.0f, i/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n+1 ].p[i]);
                    glVertex3fv( disques[ n+1 ].p[i]);

                glTexCoord2f(1.0f, (i+1)/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n+1 ].p[i+1]);
                    glVertex3fv( disques[ n+1 ].p[i+1]);
            }

                glTexCoord2f(0.0f, i/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n ].p[i]);
                    glVertex3fv( disques[ n ].p[i]);

                glTexCoord2f(1.0f, i/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n+1 ].p[i]);
                    glVertex3fv( disques[ n+1 ].p[i]);

                glTexCoord2f(0.0f, (i+1)/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n ].p[0]);
                    glVertex3fv( disques[ n ].p[0]);

                glTexCoord2f(0.0f, (i+1)/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n ].p[0]);
                    glVertex3fv( disques[ n ].p[0]);

                glTexCoord2f(1.0f, i/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n+1 ].p[i]);
                    glVertex3fv( disques[ n+1 ].p[i]);

                glTexCoord2f(1.0f, (i+1)/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n+1 ].p[0]);
                    glVertex3fv( disques[ n+1 ].p[0]);
    }
glEnd();
        //relit le premier et dernier disque si on a un circuit
            if(!circle) return;

            glBegin(GL_TRIANGLES);
            for(i = 0; i < nb_seg-1; i++)
            {

                glTexCoord2f(0.0f, i/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n ].p[i]);
                    glVertex3fv( disques[ n ].p[i]);

                glTexCoord2f(1.0f, i/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ 0 ].p[i]);
                    glVertex3fv( disques[ 0 ].p[i]);

                glTexCoord2f(0.0f, (i+1)/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n ].p[i+1]);
                    glVertex3fv( disques[ n ].p[i+1]);

                glTexCoord2f(0.0f, (i+1)/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n ].p[i+1]);
                    glVertex3fv( disques[ n ].p[i+1]);

                glTexCoord2f(1.0f, i/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ 0 ].p[i]);
                    glVertex3fv( disques[ 0 ].p[i]);

                glTexCoord2f(1.0f, (i+1)/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ 0 ].p[i+1]);
                    glVertex3fv( disques[ 0 ].p[i+1]);
            }

                glTexCoord2f(0.0f, i/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n ].p[i]);
                    glVertex3fv( disques[ n ].p[i]);

                glTexCoord2f(1.0f, i/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ 0 ].p[i]);
                    glVertex3fv( disques[ 0 ].p[i]);

                glTexCoord2f(0.0f, (i+1)/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n ].p[0]);
                    glVertex3fv( disques[ n ].p[0]);

                glTexCoord2f(0.0f, (i+1)/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ n ].p[0]);
                    glVertex3fv( disques[ n ].p[0]);

                glTexCoord2f(1.0f, i/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ 0 ].p[i]);
                    glVertex3fv( disques[ 0 ].p[i]);

                glTexCoord2f(1.0f, (i+1)/(nb_seg+0.0f));
                    glNormal3fv( n_disques[ 0 ].p[0]);
                    glVertex3fv( disques[ 0 ].p[0]);

 glEnd();
   /* for(n = 0; n < nb_d-1; n++)
    {
            glBegin(GL_TRIANGLE_STRIP);

            glTexCoord2f(0,-1/(nb_seg+0.0));glVertex3fv( disques[ n ].p[nb_seg-1]);
            glTexCoord2f(1,-1/(nb_seg+0.0));glVertex3fv( disques[ n+1 ].p[nb_seg-1]);
            for(i = 0; i < nb_seg-1; i++)
            {

                glTexCoord2f(0, i/(nb_seg+0.0));
                    glVertex3fv( disques[ n ].p[i]);

                glTexCoord2f(1, i/(nb_seg+0.0));
                    glVertex3fv( disques[ n+1 ].p[i]);

                glTexCoord2f(0, (i+1)/(nb_seg+0.0));
                    glVertex3fv( disques[ n ].p[i+1]);

                glTexCoord2f(1, (i+1)/(nb_seg+0.0));
                    glVertex3fv( disques[ n+1 ].p[i+1]);
            }
            glEnd();
    }*/
}


void RxTunnel::ComputeVBO()
{

    std::vector<vec3>&	tPosition	= tunnel_VBO.getPosition();
	std::vector<vec3>&	tNormal		= tunnel_VBO.getNormal();
	std::vector<vec2>&	tTexcoord	= tunnel_VBO.getTexcoord2();
	//enregistrement du VBO
    unsigned int n,i,nb_points;
	nb_points = nb_seg+1;
    for(n = 0; n < nb_d; n++)
    {

            for(i = 0; i < nb_points; i++)
            {
                //premier triangle du carré
                tTexcoord.push_back(vec2((float)n, i/(nb_seg+0.0f)));
                tNormal.push_back(n_disques[ n ].p[i]);
                tPosition.push_back(disques[ n ].p[i]);
				

            }

    }

	//creation des faces
	unsigned int nb_face = (nb_d-1*circle) * nb_seg*2;
	tFaces.reserve(nb_face);
    for(n = 0; n < nb_d-1; n++)
    {
            for(i = 0; i < nb_points-1; i++)
            {
				//premier triangle du carré
				tFaces.push_back(RxFace(
										(n*nb_points + i),
										((n+1)*nb_points+i),
										(n*nb_points+i+1)
										));
				//2 emme triangle du carré
				tFaces.push_back(RxFace(
										(n*nb_points + i+1),
										((n+1)*nb_points+i),
										((n+1)*nb_points+i+1)
										));
            }

    }

	if(!circle)
	{
		tunnel_VBO.Create(GL_STATIC_DRAW);
		return;
	}
	for(i = 0; i < nb_points-1; i++)
    {
				//premier triangle du carré
				tFaces.push_back(RxFace(
										(n*nb_points + i),
										((0)*nb_points+i),
										(n*nb_points+i+1)
										));
				//2 emme triangle du carré
				tFaces.push_back(RxFace(
										(n*nb_points + i+1),
										((0)*nb_points+i),
										((0)*nb_points+i+1)
										));
    }

				tunnel_VBO.Create(GL_STATIC_DRAW);

}
void RxTunnel::Draw()
{
	tunnel_VBO.Enable();
        //on dessine notre RxTunnel
	    glDrawElements(GL_TRIANGLES,tFaces.size()*3,GL_UNSIGNED_INT,tFaces[0].id);
		//glDrawArrays(GL_TRIANGLES, 0, (nb_d)*nb_seg*6);

	tunnel_VBO.Disable();
}

void RxTunnel::ReleaseDisques()
{
    if(disques != NULL)
      delete[] disques;

    if(n_disques != NULL)
       delete[] n_disques;

    nb_d = 0;
}
void RxTunnel::CreateDisques(unsigned int n)
{
    disques = new courbe[n];
    unsigned int i;
    for(i = 0; i < n ;i++)
    {
        disques[i].p.reserve(nb_seg);
        disques[i].n = nb_seg;
    }

    n_disques = new courbe[n];

    for(i = 0; i < n ;i++)
    {
        n_disques[i].p.reserve(nb_seg);
        n_disques[i].n = nb_seg;
    }

    nb_d = n;
}


