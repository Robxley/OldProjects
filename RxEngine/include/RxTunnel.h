//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#pragma once
#ifndef _RXTUNNEL_H_
#define _RXTUNNEL_H_


#include <gl/glew.h>
#include <iostream>
#include <vector>

#include "RxMath3D.h"
#include "RxVBO.h"
#include "RxObject3D.h"
#include "RxLogger.h"
//matrice d'interpolation



//
struct courbe
{
      unsigned int n ; //nombre maximum de points

      vector<vec3> p ;
      courbe(){ n = 0; }

      void Release() { p.clear() ; n = 0;}

      void AddPoint(vec3 pt)
      {
          if(p.size()> n)
          {
               RXLOGGER << "[ERROR] COURBE FULL !" << std::endl;
               return; //la courbe est pleine
          }
          else
          p.push_back(pt) ;
      }

} ;


class RxTunnel : public RxItem3D
{

    private:

        //si le RxTunnel est une boucle, ou un rally
        bool circle;

		/**partie a travailler**/
        //id des textures
        GLuint *idTex;
        unsigned int nb_Tex;

        void B_spline(courbe *p,mat4 m,int n);
        void Lisse(vec3 *p,int n, mat4 m);
        //calcul le contour du RxTunnel en fonction des points
        void CalculTunnel();


        courbe descripteur;//ensemble de points décrivant la courbe
        courbe courbe_lisse;//courbe interpolée

        //disque de chaque points interpolés et la normal au point
        courbe *disques, *n_disques;
        unsigned int nb_d; //nombre de disque
        unsigned int nb_seg; //nombre de point definissant les disques
        float radius;
        void ReleaseDisques();
        void CreateDisques(unsigned int n);

        void ComputeVBO();
        RxVBO tunnel_VBO;

		vector<RxFace> tFaces;

    public:
        RxTunnel();
        ~RxTunnel();
        //telecharge le RxTunnel à partir des données d'un fichier
        bool LoadTunnel(char *filename);
        //à partir d'un tableau de point
        bool LoadTunnel(vec3 *descripteur_c,int n, int def = 20, float r = 4, int seg = 16);


        //dessine le RxTunnel en fonction de la direction et position de la camera
        void DrawSpline();
        void DrawGLFunction(); //dessine le tunnel a base de glVertex();
        void Draw(); //dessine l'ensemble du RxTunnel (efficace pour des tunnels cours)
		void Draw(vec3 pos, vec3 dir); //dessine seulement les parties visibles du RxTunnel
        void DrawSplinePoints();
        void Log(bool b = 1);
        void setCircle(bool b){ circle = b;};
		vector<RxFace> &getFaces(){return tFaces;}
		RxVBO *getVBO(){return &tunnel_VBO;}

};


#endif
