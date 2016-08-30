
#include "RxEngine.h"

class MyEngine : public RxEngine
{
public:
	RxShader simpleShader;
	RxShader animMeshShader;
	RxCube simpleCubeMesh;
	RxFont defaultFont;
	RxFPS defaultFps;
	RxItem2D debugItem2D;
	RxButton debugButton;
	RxFBO debugFBO;
	RxSkyBox debugSkyBox;
	RxHeightMap debugHeightMap;
	RxSphere debugSphereCulling;
	RxMesh AssimpMesh;
	RxAnimMesh AnimMesh;




	unsigned int tick_counter;
	//RxSkeleton skeleton;

	//RxAssimpLoader debugAssimp;

	//RxGUI debugGUI;



	int InitGraphicObjects();
	void Display3D();
	void Display2D();
	void Frame();


};