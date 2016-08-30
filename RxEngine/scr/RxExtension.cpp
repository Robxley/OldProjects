

#include "RxExtension.h"


#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif



bool ExtensionSupportee(const char *nom)
{
#ifndef __glew_h__
    const char *extensions = glGetString(GL_EXTENSIONS);
#endif

    if(
#ifdef __glew_h__
    glewIsSupported(nom)
#else
    strstr(extensions, nom) != NULL
#endif
    )
        return true;
    else
    {
		RXLOGGER<< "[Extension] "<<nom<<" non supportee\n"<<std::endl;
        return false;
    }
}


bool InitExtensions(void)
{
    bool state = true;

    if(!ExtensionSupportee("GL_ARB_shading_language_100") ||
       !ExtensionSupportee("GL_ARB_shader_objects") ||
       !ExtensionSupportee("GL_ARB_vertex_shader") ||
       !ExtensionSupportee("GL_ARB_fragment_shader")
       /* rajoutez ici les extensions que vous utiliserez */)
    {
        state = false;
    }

    return state;
}