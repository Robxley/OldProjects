//#============================================================#
//#        Copyright © 2010, RXENGINE.			               #
//#	  Fichier :											       #
//#	  Description : 									       #
//#														       #
//#														       #
//#============================================================#

#include "RxPhysic.h"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


using namespace std;


RxPhysic::RxPhysic()
{
    gPhysicsSDK = NULL;
    gScene = NULL;
    gDefaultGravity = NxVec3(0.0f,-9.8f,0.0f);
    FElasticMaterial = 0.01f;
    FStaticFrictionMaterial = 1.5f;
    FDynamicFrictionMaterial = 0.2f;
    elapseTime = NULL;
    gSelectedActor = NULL;
    initOK = 0;
}

RxPhysic::~RxPhysic()
{
    ReleaseScene();
}
void RxPhysic::setDefautMaterial(float _FElastic, float _FSFriction, float _FDFriction)
{
    FElasticMaterial = _FElastic;
    FStaticFrictionMaterial = _FSFriction;
    FDynamicFrictionMaterial = _FDFriction;
}
void RxPhysic::setGravity(float x, float y, float z)
{
    gDefaultGravity = NxVec3(x,y,z);
}
void RxPhysic::setElapseTime(unsigned int *_elapseTime)
{
    elapseTime = _elapseTime;
}
bool RxPhysic::InitScene()
{

//Create the physics SDK
    gPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION);
    if (!gPhysicsSDK)  return 0;

// Set the physics parameters
	gPhysicsSDK->setParameter(NX_SKIN_WIDTH, 0.01f);

	// Set the debug visualization parameters
	/*gPhysicsSDK->setParameter(NX_VISUALIZATION_SCALE, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_SHAPES, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_ACTOR_AXES, 1);
	gPhysicsSDK->setParameter(NX_VISUALIZE_COLLISION_FNORMALS, 1);*/
	gPhysicsSDK->setParameter(NX_CONTINUOUS_CD, true);
   gPhysicsSDK->setParameter(NX_CCD_EPSILON, 0.01f);



    // Create the scene
    NxSceneDesc sceneDesc;
    sceneDesc.gravity               = gDefaultGravity;
    sceneDesc.simType		= NX_SIMULATION_SW;
	sceneDesc.userContactReport = &contactReport;

    gScene = gPhysicsSDK->createScene(sceneDesc);

    // Create the default material
    NxMaterial* defaultMaterial = gScene->getMaterialFromIndex(0);
    defaultMaterial->setRestitution(FElasticMaterial);
    defaultMaterial->setStaticFriction(FStaticFrictionMaterial);
    defaultMaterial->setDynamicFriction(FDynamicFrictionMaterial);

    initOK = 1;

	if (gScene)
	{
		StartPhysics();
		gScene->setGroupCollisionFlag(RX_GROUP_COLLIDABLE,RX_GROUP_NON_COLLIDABLE,false);
		gScene->setGroupCollisionFlag(RX_GROUP_COLLIDABLE,RX_CAMERA_GROUP,false);
		gScene->setGroupCollisionFlag(RX_GROUP_NON_COLLIDABLE,RX_CAMERA_GROUP,true);

	}


    return 1;
}

NxActor* RxPhysic::AddGroundPlane(NxVec3 p, NxVec3 n, const char *name)
{
    if(!initOK) return NULL;
    // Create a plane with default descriptor
    NxPlane plane;
	plane.set(p,n);
	//attention NxPlaneShapeDesc equation de la forme ax + by + cz = d;
	//et Nxplane equation de la forme ax + by + cz + d = 0.
    NxPlaneShapeDesc planeDesc;
    planeDesc.d = -plane.d;
    planeDesc.normal = plane.normal;
	
	
    NxActorDesc actorDesc;
    actorDesc.shapes.pushBack(&planeDesc);
	actorDesc.name = name;

	

    //actorDesc.density = 0;

    return gScene->createActor(actorDesc);
}
NxActor* RxPhysic::AddGroundPlane(float x, float y, float z, float w, const char *name)
{
	if(!initOK) return NULL;
    // Create a plane with default descriptor
    NxPlane plane;
    NxPlaneShapeDesc planeDesc;
	//attention NxPlaneShapeDesc equation de la forme ax + by + cz = d;
	//et Nxplane equation de la forme ax + by + cz + d = 0.
    planeDesc.d = -w; 
	planeDesc.normal = NxVec3(x,y,z);
	
	
    NxActorDesc actorDesc;
    actorDesc.shapes.pushBack(&planeDesc);
	actorDesc.name = name;

	

    //actorDesc.density = 0;

    return gScene->createActor(actorDesc);
}

NxActor* RxPhysic::AddCapsule(NxReal radius , NxReal height, NxVec3 p, NxReal density, const char *name )
{
    if(!initOK) return NULL;

    NxActorDesc actorDesc;
    NxBodyDesc bodyDesc;

    // The actor has one shape, a capsule
    NxCapsuleShapeDesc capsuleDesc;

    capsuleDesc.height = height;
    capsuleDesc.radius = radius;
    actorDesc.shapes.pushBack(&capsuleDesc);

	actorDesc.body = &bodyDesc;
    actorDesc.density = density;

    actorDesc.globalPose.t = p;
    actorDesc.name = name;
    return gScene->createActor(actorDesc);


}
NxActor* RxPhysic::AddSphere(NxReal radius, NxVec3 p, NxReal density, const char *name)
{
       if(!initOK) return NULL;

        NxBodyDesc bodyDesc;
        NxActorDesc actorDesc;

        NxSphereShapeDesc sphereDesc;
        sphereDesc.radius = radius;

        actorDesc.shapes.pushBack(&sphereDesc);


		actorDesc.body = &bodyDesc;
        actorDesc.density         = density;
        actorDesc.globalPose.t    = p; //position at the origin.
        actorDesc.name = name;
        return gScene->createActor(actorDesc);


}
NxActor* RxPhysic::AddBox(NxVec3 size , NxVec3 pos, NxReal density, const char *name )
{
     if(!initOK) return NULL;
    // Add a single-shape actor to the scene
    NxActorDesc actorDesc;
    NxBodyDesc bodyDesc;

    // The actor has one shape, a box, 1m on a side
    NxBoxShapeDesc boxDesc;
    boxDesc.dimensions.set(size);
    actorDesc.shapes.pushBack(&boxDesc);


	actorDesc.body = &bodyDesc;
    actorDesc.density 	= density;
    actorDesc.globalPose.t 	= pos;
	actorDesc.name = name;
    assert(actorDesc.isValid());
    NxActor *pActor = gScene->createActor(actorDesc);
    assert(pActor);
    
    // //create actor with no shapes
    //NxShape* const *shape = pActor->getShapes();
    //NxBoxShape *boxShape = shape[0]->isBox();
    //assert(boxShape);
    //pActor->releaseShape(*boxShape);

    return pActor;
}

void RxPhysic::StartPhysics()
{
	

    if(!initOK) return;
    // Get results from gScene->simulate(deltaTime)

   // while (!gScene->fetchResults(NX_RIGID_BODY_FINISHED, false));

    unsigned int t = computeStep;
    if(elapseTime != NULL) t = (*elapseTime);


    gScene->simulate(t*0.001f);
	
	//RXLOGGER<<"erreur flushStream"<<endl;
    gScene->flushStream();
	

	//while (!gScene->fetchResults(NX_RIGID_BODY_FINISHED, true));

	gScene->fetchResults(NX_RIGID_BODY_FINISHED, true);
}




void RxPhysic::SelectNextActor()
{
    if(!initOK) return;

  NxU32 nbActors = gScene->getNbActors();
  NxActor** actors = gScene->getActors();
  for(NxU32 i = 0; i < nbActors; i++)
  {
         if (actors[i] == gSelectedActor)
       {
          NxU32 j = 1;
            gSelectedActor = actors[(i+j)%nbActors];
            //gSelectedActor->getMass
            //while (!IsSelectable(gSelectedActor))
            while (!gSelectedActor->getMass())
          {
               j++;
               gSelectedActor = actors[(i+j)%nbActors];
             }
           break;
         }
   }
}

void RxPhysic::ReleaseScene()
{
    if (gScene)
    {
        //GetPhysicsResults();  // Make sure to fetchResults() before shutting down
        gPhysicsSDK->releaseScene(*gScene);
    }
    if (gPhysicsSDK)  gPhysicsSDK->release();

}
NxVec3 RxPhysic::ApplyForce(NxVec3 _direction, NxReal _intensity, NxActor *actor)
{
    if(!initOK) return NxVec3(0.0);

    if (actor==NULL) NxVec3(0.0,0.0,0.0);

    float _elapseTime = computeStep;
    if(elapseTime != NULL) _elapseTime = (float)(*elapseTime);

    NxVec3 forceVec = _intensity*_direction*_elapseTime;
    actor->addForce(forceVec);
    return forceVec;


        return NxVec3(0,0,0);
}

 float* RxPhysic::getGLMat(NxActor *actor , float *glmat)
{
    if(!initOK) return NULL;
    //NxShape* shape = NULL;
    NxMat34 pose;    // 3x4 column major PhysX matrix

    pose = actor->getGlobalPose();

    NxMat33 orient = pose.M;
    NxVec3 pos = pose.t;

   // float glmat[16];    // 4x4 column major OpenGL matrix
    orient.getColumnMajorStride4(glmat);
    pos.get(&(glmat[12]));

    // clear the elements we don't need:
    glmat[3] = glmat[7] = glmat[11] = 0.0f;
    glmat[15] = 1.0f;

    return glmat;
}

void RxPhysic::DrawGLDebug(bool shadow)
{
    // Render all the actors in the scene
    NxU32 nbActors = gScene->getNbActors();
    NxActor** actors = gScene->getActors();
    while (nbActors--)
    {
        NxActor* actor = *actors++;
		string a,b;
		//if(actor->getName() && string(actor->getName()) == string("draw"))
        DrawActor(actor, NULL, false);

        // Handle shadows
        if (shadow)
        {
			DrawActorShadow(actor, false);
        }
    }
}

//verts : tableau contenant les sommets des triangles, avec nbv le nombre total de ces sommets.
//indices : liste des indices definissant les triangles à partir des verts, et nbt représente le nombre de triangle
//qui compose notre RxMesh ( on a donc le tableau indices de taille nbt*3)
//l'orientation des faces est importantes pour le calcul des normals qui doivent ce diriger vers l'exterieur de l'objet
NxActor* RxPhysic::AddTriangleMesh(NxVec3 *verts, NxU32 nbVerts, NxU32 *indices, NxU32 nbTriangles, NxVec3 pos, NxReal density, const char *name)
{


	// Create descriptor for triangle RxMesh
	NxTriangleMeshDesc triangleMeshDesc;

	triangleMeshDesc.numVertices			= nbVerts;  //nombre de vertice
	triangleMeshDesc.pointStrideBytes		= sizeof(NxVec3);
	triangleMeshDesc.points				    = verts;
	triangleMeshDesc.numTriangles			= nbTriangles;
	triangleMeshDesc.flags					= 0;
	triangleMeshDesc.triangles				= indices;
	triangleMeshDesc.triangleStrideBytes	= 3 * sizeof(NxU32);

	


	// The actor has one shape, a triangle RxMesh
	NxInitCooking();
	MemoryWriteBuffer buf;

	bool status = NxCookTriangleMesh(triangleMeshDesc, buf);
	NxTriangleMesh* pMesh;
	if (status)
	{
		pMesh = gPhysicsSDK->createTriangleMesh(MemoryReadBuffer(buf.data));
	}
	else
	{
	
		assert(false);
		
		pMesh = NULL;
	}
	NxCloseCooking();
	// Create TriangleMesh above code segment.

	NxTriangleMeshShapeDesc tmsd;
	tmsd.meshData		= pMesh;
	tmsd.userData		= &triangleMeshDesc;
	tmsd.meshPagingMode = NX_MESH_PAGING_AUTO;
	
	NxActorDesc actorDesc;
	

	
	assert(tmsd.isValid());
	actorDesc.shapes.pushBack(&tmsd);
	//Dynamic triangle RxMesh don't be supported anymore. So body = NULL
	
	//objet dynamique
	NxBodyDesc  bodyDesc;	
	actorDesc.body = NULL;
	actorDesc.globalPose.t	= pos;

    actorDesc.name = name;
	if (pMesh)
	{
		// Save RxMesh in userData for drawing
		pMesh->saveToDesc(triangleMeshDesc);
		//
		assert(actorDesc.isValid());
		NxActor *actor = gScene->createActor(actorDesc);
		assert(actor);
		return actor;
	}else
	{
		RXLOGGER<<"[ERREUR][RxPhysic] creation actor mesh"<<endl;
	}

	return NULL;
}


NxActor* RxPhysic::AddCylinderMesh(unsigned int ndiv,NxVec3 size, NxVec3 pos , NxReal density, const char *name)
{
    NxVec3 * verts = new NxVec3[ndiv*2];
	NxReal x,z;

    NxF32 step = 0;
	unsigned int n = 0;
	for(n = 0; n < ndiv; n++)
	{
		x = cos(step)* size.x;
		z = sin(step)* size.z;
		verts[n] = NxVec3(x,-0.5f*size.y,z);
		verts[n+ndiv] = NxVec3(x,0.5f*size.y,z);
		step+=(NxTwoPiF32/ndiv);
	}
	NxActor * pActor = AddConvexAutoMesh(verts,ndiv*2,pos,density,name);
	delete[] verts;
    return pActor;

}
NxActor* RxPhysic::AddHeightMap(unsigned int * hmap, unsigned int w, unsigned int h, NxVec3 scale, NxVec3 pos, const char *name)
{
		NxHeightFieldDesc heightFieldDesc;

		heightFieldDesc.nbColumns           = h;
		heightFieldDesc.nbRows              = w;
		heightFieldDesc.verticalExtent      = -1000;
		heightFieldDesc.convexEdgeThreshold = 0;

		//heightFieldDesc.
   
	
		// allocate storage for samples
		heightFieldDesc.samples             = new NxU32[h*w];
		heightFieldDesc.sampleStride        = sizeof(NxU32);

		NxU8* currentByte = (NxU8*)heightFieldDesc.samples;
		for (NxU32 i = 0; i < w; i++)
		{
			for(NxU32 j = 0; j < h; j++)
			{
				NxHeightFieldSample* currentSample = (NxHeightFieldSample*)currentByte;

				currentSample->height         = hmap[j*w+i]; //desired height value. Singed 16bit integer
				//currentSample->materialIndex0 = gMaterial0;
				//currentSample->materialIndex1 = gMaterial1;

				currentSample->tessFlag = 1;

				currentByte += heightFieldDesc.sampleStride;
			}
		}

		NxHeightField* heightField = gScene->getPhysicsSDK().createHeightField(heightFieldDesc);
		// data has been copied, we can free our buffer
		delete[] heightFieldDesc.samples;
		
		NxHeightFieldShapeDesc heightFieldShapeDesc;

		heightFieldShapeDesc.heightField     = heightField;
		heightFieldShapeDesc.heightScale     = scale.y;
		heightFieldShapeDesc.rowScale        = scale.x;
		heightFieldShapeDesc.columnScale     = scale.z;
		heightFieldShapeDesc.materialIndexHighBits = 0;
		heightFieldShapeDesc.holeMaterial = 2;
		heightFieldShapeDesc.meshFlags = 0;

		NxActorDesc actorDesc;

		actorDesc.shapes.pushBack(&heightFieldShapeDesc);
		actorDesc.globalPose.t = pos;
	
		actorDesc.name = name;
		NxActor* newActor = gScene->createActor(actorDesc);
		//newActor->setGlobalPosition(pos);


	return newActor;
}

NxActor* RxPhysic::AddConvexAutoMesh(NxVec3 *verts,unsigned int nbVerts, const NxVec3 pos, NxReal density, const char *name)
{

    NxBodyDesc bodyDesc;
	NxConvexMeshDesc convexDesc;

	// Create descriptor for convex mesh
	
    convexDesc.numVertices			= nbVerts;
    convexDesc.pointStrideBytes	= sizeof(NxVec3);
    convexDesc.points				= verts;
	convexDesc.flags				= NX_CF_COMPUTE_CONVEX;

	NxConvexShapeDesc convexShapeDesc;
	//convexShapeDesc.localPose.t		= pos;
	convexShapeDesc.userData		= &convexDesc;
 
	
	// Two ways on cooking mesh: 1. Saved in memory, 2. Saved in file	
	NxInitCooking();
	// Cooking from memory
    MemoryWriteBuffer buf;
    bool status = NxCookConvexMesh(convexDesc, buf);
	//
	// Please note about the created Convex Mesh, user needs to release it when no one uses it to save memory. It can be detected
	// by API "meshData->getReferenceCount() == 0". And, the release API is "gPhysicsSDK->releaseConvexMesh(*convexShapeDesc.meshData);"
	//
	NxConvexMesh *pMesh			= gPhysicsSDK->createConvexMesh(MemoryReadBuffer(buf.data));
	assert(pMesh);
    convexShapeDesc.meshData	= pMesh;
	NxCloseCooking();

    if (pMesh)
    {
        // Save mesh in userData for drawing.
		pMesh->saveToDesc(convexDesc);
		//
		NxActorDesc actorDesc;
		assert(convexShapeDesc.isValid());
        actorDesc.shapes.pushBack(&convexShapeDesc);
	    actorDesc.body		= &bodyDesc;
		actorDesc.density	= density;
	    actorDesc.name = name;
        actorDesc.globalPose.t = pos;
		assert(actorDesc.isValid());
		NxActor* actor = gScene->createActor(actorDesc);
		assert(actor);	
		return actor;
    }

    return NULL;
}

NxActor* RxPhysic::AddConvexTriangleMesh(NxVec3 *verts,unsigned int nbVerts, NxU32 *indices, NxU32 nbTriangles, const NxVec3 pos, NxReal density, const char *name )
{
	// Create descriptor for convex mesh
	NxConvexMeshDesc convexDesc;
	convexDesc.numVertices			= nbVerts;
	convexDesc.pointStrideBytes		= sizeof(NxVec3);
	convexDesc.points				= verts;
	convexDesc.numTriangles			= nbTriangles;
	convexDesc.triangleStrideBytes  = 3 * sizeof(NxU32);
	convexDesc.triangles			= indices;
	convexDesc.flags				= NX_CF_COMPUTE_CONVEX;
	
////
	NxInitCooking();
	MemoryWriteBuffer buf;
	bool status = NxCookConvexMesh(convexDesc, buf);
	NxConvexMesh* pMesh;
	if (status)
	{
		pMesh = gPhysicsSDK->createConvexMesh(MemoryReadBuffer(buf.data));
	}
	else
	{
		assert(false);
		pMesh = NULL;
	}
	NxCloseCooking();
	///
	
	if(status)
	{

		NxConvexShapeDesc convexShapeDesc;
		convexShapeDesc.meshData = pMesh;
    
		if(convexShapeDesc.meshData)
		{
			NxActorDesc ActorDesc;
			ActorDesc.shapes.pushBack(&convexShapeDesc);
			NxBodyDesc BodyDesc;
			ActorDesc.body			= &BodyDesc;
			ActorDesc.density		= density;
			ActorDesc.globalPose.t  = pos;
			ActorDesc.name = name;
			return gScene->createActor(ActorDesc);
		}
	}
	
	return NULL;
}



void RxPhysic::SetActorCollisionGroup(NxActor* actor, NxCollisionGroup group)
{
	if(!actor) return;
    NxShape*const* shapes = actor->getShapes();
    NxU32 nShapes = actor->getNbShapes();
    while (nShapes--)
    {
        shapes[nShapes]->setGroup(group);
	}
}



void RxPhysic::setContactReport(NxActor * pActor)
{
	contactReport.setActorReport(pActor);
}

/****************** RxPhysicMaterial *************************/

	
RxPhysXMaterial::RxPhysXMaterial(NxScene *_scene)
{
	   ID = 0;
	   material = NULL;;
	   scene = _scene;
}
RxPhysXMaterial::RxPhysXMaterial(RxPhysic *physicScene)
{
	   ID = 0;
	   material = NULL;;
	   scene = physicScene->getScene();
}

bool RxPhysXMaterial::Create(float Elastic, float staticFriction, float dynamicFriction)
{
	if(scene == NULL) return 0;

	NxMaterialDesc materialDesc;    
	materialDesc.restitution = Elastic;    
	materialDesc.staticFriction = staticFriction;    
	materialDesc.dynamicFriction = dynamicFriction; 

	material = scene->createMaterial(materialDesc);
	if (material == NULL) return 0;

	ID = material->getMaterialIndex();

	return 1;
}

void RxPhysXMaterial::set(float Elastic, float staticFriction, float dynamicFriction)
{
	if(material == NULL) return;

	material->setRestitution(Elastic);
	material->setStaticFriction(staticFriction);
	material->setDynamicFriction(dynamicFriction);
}
		
void RxPhysXMaterial::setElastic(float e)
{

	if(material == NULL) return;

	material->setRestitution(e);


}
void RxPhysXMaterial::setStaticFriction(float s)
{
	if(material == NULL) return;
	material->setStaticFriction(s);
}
void RxPhysXMaterial::setDynamicFriction(float d)
{
	if(material == NULL) return;
	material->setDynamicFriction(d);
}
		
NxMaterial* RxPhysXMaterial::getMaterial(){return material;}

void RxPhysXMaterial::setMaterial(NxMaterial *m)
{
	//if(material == NULL)
		material = m;
	
}

void RxPhysXMaterial::setScene(NxScene *_scene)
{
	scene = _scene;
}

void RxPhysXMaterial::setScene(RxPhysic *physicScene)
{
	scene = physicScene->getScene();
}


