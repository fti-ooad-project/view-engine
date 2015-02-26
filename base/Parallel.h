#ifndef PARALLEL_H
#define PARALLEL_H
#include <SDL2/SDL_thread.h>
#include <thread>
#include <chrono>
#define sleep( x ) std::this_thread::sleep_for( std::chrono::milliseconds( x ) )
class Parallel
{
private:
	static int start_th( void *in )
	{
		Parallel *run = static_cast< Parallel* >( in );
		run->run();
		return 0;
	}
protected:
	bool _working = true;
	SDL_Thread *_thread;
public:
	bool working()
	{
		return _working;
	}
	void stop()
	{
		_working = false;
	}
	void start()
	{
		_thread = SDL_CreateThread( Parallel::start_th , "thread" , this );
	}
	virtual void run() = 0;
};
#endif // PARALLEL_H
