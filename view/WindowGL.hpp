#ifndef WINDOWGL_HPP
#define WINDOWGL_HPP
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <thread>
#include <chrono>
#include "base/Parallel.h"
#include "base/RBase.h"
#include "base/REventer.h"
#include <functional>
class RWindowGL : public RTimer , public Parallel , public RInitable
{
public:
	typedef std::function< void( int , int ) > FrameFunc;
	typedef std::function< void() > ReleaseFunc;
protected:
	keystate	__key_state[MAX_KEY] = {0};
	keystate	__mouse_state[3] = {0};
	f2		__mouse_pos = f2( 0.0f , 0.0f );
	int _screen_width = 512 , _screen_height = 512;
	bool _cursor_hold = false;
	SDL_Window *mainwindow;
	bool mouse_holded = false;
	bool ignore_mmove = false;
	FrameFunc _func;
	ReleaseFunc _relf;
	void holdCursor()
	{
		SDL_WarpMouseInWindow( mainwindow , _screen_width / 2 , _screen_height / 2 );
	}
	void setMouse( int x , int y )
	{
		__mouse_pos.x() = -1.0f + float( x ) / _screen_width * 2.0f;
		__mouse_pos.y() = 1.0f - float( y ) / _screen_height * 2.0f;
	}
public:
	void init( FrameFunc func , ReleaseFunc relf )
	{
		if( isInited() ) return;
		setInited( true );
		_func = func;
		_relf = relf;
		start();
	}
	void release() override
	{
		if( !isInited() ) return;
		setInited( false );
	}
	~RWindowGL() override
	{
		release();
	}
public:
	void run() override
	{
		int err;
		err = SDL_Init( SDL_INIT_EVERYTHING );
		SDL_GLContext maincontext;
		mainwindow = SDL_CreateWindow( "PROGRAM_NAME" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , 512 , 512 , SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );
		maincontext = SDL_GL_CreateContext( mainwindow );
		glewInit();
		glEnable( GL_POINT_SMOOTH );
		glDisable( GL_CULL_FACE );
		//glCullFace( GL_BACK );
		//glFrontFace( GL_CCW );
		SDL_GL_SetAttribute( SDL_GL_RED_SIZE , 5 );
		SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE , 5 );
		SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE , 5 );
		SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE , 16 );
		SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER , 1 );
		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LESS );
		glPatchParameteri( GL_PATCH_VERTICES , 3 );
		SDL_GL_SetSwapInterval( 1 );
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
						if( e.button.button == 1 )
							mouse_holded = true;
					}
					break;
					case SDL_MOUSEBUTTONUP:
					{
						__mouse_state[e.button.button-1] = false;
						updateMouse();
						if( e.button.button == 1 )
							mouse_holded = false;
					}
					break;
					case SDL_MOUSEMOTION:
					{
						if( ignore_mmove )
						{
							ignore_mmove = false;
							continue;
						}
						updateMouse();
					}
					break;
				}
			}
			//__eventer->update( this->__key_state , this->__mouse_state , &this->__mouse_pos );
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
};
#endif // WINDOWGL_HPP
