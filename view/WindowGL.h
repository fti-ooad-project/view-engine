#ifndef WINDOWGL_HPP
#define WINDOWGL_HPP
#include <thread>
#include <chrono>
#include "../base/Parallel.h"
#include "../base/RBase.h"
#include "../base/REventer.h"
#include <functional>
#include <SDL2/SDL.h>
class RWindowGL : public RTimer , public Parallel , public RInitable
{
public:
	typedef std::function< void( int , int ) > FrameFunc;
	typedef std::function< void() > ReleaseFunc;
protected:
	keystate	__key_state[MAX_KEY];
	keystate	__mouse_state[3];
	f2		__mouse_pos;
	float _mwheel;
	int _screen_width , _screen_height;
	SDL_Window *mainwindow;
	FrameFunc _func;
	ReleaseFunc _relf;
	REventer *_eventer;
public:
	void init( FrameFunc  , ReleaseFunc  , REventer * );
	void release() override;
	~RWindowGL() override;
public:
	void run() override;
};
#endif // WINDOWGL_HPP
