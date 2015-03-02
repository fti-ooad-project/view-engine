#ifndef GL_H
#define GL_H
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
inline void assertGL( int mark )
{
	if( int i = glGetError() )
	{
#ifdef RLOG
		std::cout << "ogl err at " << mark << ":" << i << "\n";
#endif
		throw std::logic_error( "opengl assert error" );
	}
}
#endif // GL_H
