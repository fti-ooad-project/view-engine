#include "../view/WindowGL.h"
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
void RWindowGL::init(FrameFunc func, ReleaseFunc relf, REventer *eventer)
{
	if (isInited())
		return;
	_screen_width = 512;
	_screen_height = 512;
	setInited(true);
	_func = func;
	_relf = relf;
	_eventer = eventer;
	ito(2)
		memset(__key_state, 0, MAX_KEY * sizeof(keystate));
	ito(2)
		memset(__mouse_state, 0, 3 * sizeof(keystate));
	start();
}
void RWindowGL::release()
{
	if( !isInited() ) return;
	setInited( false );
}
RWindowGL::~RWindowGL()
{
	release();
}
void RWindowGL::run()
{
	int err;
	err = SDL_Init( SDL_INIT_EVERYTHING );
	SDL_GLContext maincontext;
	mainwindow = SDL_CreateWindow( "PROGRAM_NAME" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 512 , 512 , SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
	maincontext = SDL_GL_CreateContext( mainwindow );
	glewInit();
	glEnable( GL_POINT_SMOOTH );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glFrontFace( GL_CCW );
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE , 5 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE , 5 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE , 5 );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE , 16 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER , 1 );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glPatchParameteri( GL_PATCH_VERTICES , 3 );
	SDL_GL_SetSwapInterval( 0 );
	glDisable( GL_BLEND );
	while( _working )
	{
		SDL_Event e;
		auto updateMouse = [this,&e]()
		{
			this->__mouse_pos = f2( -2.0f * e.button.x / this->_screen_width + 1.0f , 2.0f * e.button.y / this->_screen_height - 1.0f );
		};
		while( SDL_PollEvent( &e ) != 0 )
		{
			switch( e.type )
			{
				case SDL_QUIT:
				goto exit;
				case SDL_KEYDOWN:
				{
					switch( e.key.keysym.sym )
					{
						case 27:
						goto exit;
					}
					this->__key_state[e.key.keysym.scancode] = true;
				}
				case SDL_KEYUP:
				{
					if( e.key.state == SDL_RELEASED )
					{
						this->__key_state[e.key.keysym.scancode] = false;
					}
				}
				break;
				case SDL_MOUSEBUTTONDOWN:
				{
					__mouse_state[e.button.button - 1] = true;
					updateMouse();
				}
				break;
				case SDL_MOUSEBUTTONUP:
				{
					__mouse_state[e.button.button-1] = false;
					updateMouse();
				}
				break;
				case SDL_MOUSEMOTION:
				{
					updateMouse();
				}
				break;
			}
		}
		_eventer->update( this->__key_state , this->__mouse_state , &this->__mouse_pos );
		_eventer->call();
		SDL_GetWindowSize( mainwindow , &_screen_width , &_screen_height );
		_func( _screen_width , _screen_height );
		updateTime();
		SDL_GL_SwapWindow( mainwindow );
	}
	exit:
	_relf();
	SDL_GL_DeleteContext( maincontext );
	SDL_DestroyWindow( mainwindow );
	SDL_Quit();
	release();
	return;
}



