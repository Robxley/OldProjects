



// Windows memory leak checkin
#ifdef _DEBUG
	//#include <vld.h>
#endif

#include "../tests.h"


int main( int argc, char** argv )
{

     const char app_name[] = "RxEngine Demo SDLGL";

	RxSDLOpenglContext *sdlopenglContext;
	sdlopenglContext = new RxSDLOpenglContext(app_name,1024,768);
	if(!sdlopenglContext->Init())
	{
		delete sdlopenglContext;
		return EXIT_SUCCESS;
	}
    MyEngine * engine = new MyEngine();
	if( engine  == NULL)
	{
		return EXIT_SUCCESS;
	}
	engine->Init(sdlopenglContext);
    engine->Loop();


    delete engine;
	delete sdlopenglContext;


    return (EXIT_SUCCESS);
}
