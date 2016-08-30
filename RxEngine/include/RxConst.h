//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#
#pragma once


#ifndef _RXCONST_H_
#define _RXCONST_H_


//Debug
#ifdef _DEBUG
#define RX_VERBOSE  0
#endif


#define RX_ERROR 0
#define RX_OK 1

//enable
#define RX_ON		1
#define RX_OFF		0


#define RX_RIGHT		0
#define RX_LEFT			1
#define RX_FRONT		2
#define RX_BACK			3
#define RX_TOP			4
#define RX_BOTTOM		5

#define RX_UNVISIBLE	0
#define RX_VISIBLE		1

#define RX_INFINITY 10.0f
#define RX_INVINFINITY 0.00001f


#define RX_VERTEX_ATTRIB_INDEX 0
#define RX_COLOR_ATTRIB_INDEX  1
#define RX_COORD0_ATTRIB_INDEX 2
#define RX_NORMAL_ATTRIB_INDEX 3
#define RX_COORD1_ATTRIB_INDEX 4
#define RX_COORD2_ATTRIB_INDEX 5
#define RX_DATA0_ATTRIB_INDEX  6
#define RX_DATA1_ATTRIB_INDEX  7
#define RX_DATA2_ATTRIB_INDEX  8

#define RX_VERTEX_ATTRIB_NAME  "in_Vertex"
#define RX_COLOR_ATTRIB_NAME   "in_Color"
#define RX_COORD0_ATTRIB_NAME  "in_Coord0"
#define RX_NORMAL_ATTRIB_NAME  "in_Normal"
#define RX_COORD1_ATTRIB_NAME  "in_Coord1"
#define RX_COORD2_ATTRIB_NAME  "in_Coord2"
#define RX_DATA0_ATTRIB_NAME   "in_Data0"
#define RX_DATA1_ATTRIB_NAME   "in_Data1"
#define RX_DATA2_ATTRIB_NAME   "in_Data2"

#define RX_TRANSFORM_MODEL_UNIFORM_NAME "transform"

typedef unsigned int RxIDTexture ;

enum RX_ITEMS_3D{ //liste des objets 3D
	RX_DEFAULT_ITEM3D,
	RX_SPHERE_ITEM3D,
	RX_CUBE_ITEM3D,
	RX_CAPSULE_ITEM3D,
	RX_CYLINDER_ITEM3D,
	RX_MESH_ITEM3D,
	RX_PLANE_ITEM3D,
	RX_SURFACE_ITEM3D,
	RX_HEIGHTMAP_ITEM3D,
	RX_WATER_ITEM3D,

};

const char RX_ITEMS_3D_STRING[][32] =
{
	"RX_DEFAULT_ITEM3D",
	"RX_SPHERE_ITEM3D",
	"RX_CUBE_ITEM3D",
	"RX_CAPSULE_ITEM3D",
	"RX_CYLINDER_ITEM3D",
	"RX_MESH_ITEM3D",
	"RX_PLANE_ITEM3D",
	"RX_SURFACE_ITEM3D",
	"RX_HEIGHTMAP_ITEM3D",
	"RX_WATER_ITEM3D"
};

#define LoremIpsum "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Sed non risus. Suspendisse lectus tortor, dignissim sit amet, adipiscing nec, ultricies sed, dolor. Cras elementum ultrices diam. Maecenas ligula massa, varius a, semper congue, euismod non, mi. Proin porttitor, orci nec nonummy molestie, enim est eleifend mi, non fermentum diam nisl sit amet erat. Duis semper. Duis arcu massa, scelerisque vitae, consequat in, pretium a, enim. Pellentesque congue. Ut in risus volutpat libero pharetra tempor. Cras vestibulum bibendum augue. Praesent egestas leo in pede. Praesent blandit odio eu enim. Pellentesque sed dui ut augue blandit sodales. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia Curae; Aliquam nibh. Mauris ac mauris sed pede pellentesque fermentum. Maecenas adipiscing ante non diam sodales hendrerit. Ut velit mauris, egestas sed, gravida nec, ornare ut, mi. Aenean ut orci vel massa suscipit pulvinar. Nulla sollicitudin. Fusce varius, ligula non tempus aliquam, nunc turpis ullamcorper nibh, in tempus sapien eros vitae ligula. Pellentesque rhoncus nunc et augue. Integer id felis. Curabitur aliquet pellentesque diam. Integer quis metus vitae elit lobortis egestas. Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Morbi vel erat non mauris convallis vehicula. Nulla et sapien. Integer tortor tellus, aliquam faucibus, convallis id, congue eu, quam. Mauris ullamcorper felis vitae erat. Proin feugiat, augue non elementum posuere, metus purus iaculis lectus, et tristique ligula justo vitae magna. Aliquam convallis sollicitudin purus. Praesent aliquam, enim at fermentum mollis, ligula massa adipiscing nisl, ac euismod nibh nisl eu lectus. Fusce vulputate sem at sapien. Vivamus leo. Aliquam euismod libero eu enim. Nulla nec felis sed leo placerat imperdiet. Aenean suscipit nulla in justo. Suspendisse cursus rutrum augue. Nulla tincidunt tincidunt mi. Curabitur iaculis, lorem vel rhoncus faucibus, felis magna fermentum augue, et ultricies lacus lorem varius purus. Curabitur eu amet.\0"
#define LoremIpsum_Small "Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Sed non risus. Suspendisse lectus tortor, dignissim sit amet, adipiscing nec, ultricies sed, dolor. Cras elementum ultrices diam. Maecenas ligula massa, varius a, semper congue, euismod non, mi.\0"


#define LoremFrench "«On va au Mondial avec l'envie de se faire plaisir». Voilà l'objectif des Bleus, à en croire Antonin Rouzier, qui dit certainement vrai, mais ne dit pas tout. La raison : «Se mettre la pression, cela ne nous réussit pas. On veut faire comme à l'Euro, ne pas se prendre la tête.» L'équipe de France préfère arriver masquée, sans crier son ambition. Pour mieux surprendre, comme elle l'avait fait l'année dernière en Turquie ? Ses prestations mitigées depuis sa médaille d'argent à l'Euro, ont contribué à cette philosophie de discrétion. Les Tricolores ont alterné le très bon et le piteux, livrant un condensé de leur été lors du Tournoi de France. «Pour la confiance, on va retenir le bon ! On n'a jamais lâché un point contre les Bulgares», note Rouzier, «pas inquiet»."
#define LoremFrench_Small "«On va au Mondial avec l'envie de se faire plaisir». Voilà l'objectif des Bleus, à en croire Antonin Rouzier, qui dit certainement vrai, mais ne dit pas tout. La raison : «Se mettre la pression, cela ne nous réussit pas. On veut faire comme à l'Euro, ne pas se prendre la tête.»"



#endif