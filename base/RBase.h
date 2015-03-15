#ifndef _RBase_H_
#define _RBase_H_
//#include <list>
//#include "vector"
#include <time.h>
//#include <functional>
#include "../linalg/vec.h"
#include "../linalg/mat.h"
#include <memory>
//#include <SDL2/SDL.h>
#define MARK printf( "mark\n" );
typedef unsigned int uint;
typedef unsigned short ushort;
#define MAX_KEY 0x100
#define MIN( x , y ) x < y ? x : y
#define ifor( i , x , y ) for( int i = x; i < y; ++i )
#define ito( y ) ifor( i , 0 , y )
struct RSize
{
	uint _w , _h;
	RSize(uint w = 0, uint h = 0);
	RSize(const RSize &size);
};
/*class Log
{
static std::fstream file;
public:
};
std::fstream Log::file;*/
#ifdef _WIN32
	#define TIMERK 1.0f
#else
	#define TIMERK 0.001f
#endif
#define LOG std::cout
class RTimer
{
public:
	float _dt = 0.0f , _last_time = 0.0f , _cur_time = 0.0f;
	void updateTime();
};
#if RDEBUG
        #include "stdio.h"
#endif
class RInitable
{
private:
	bool _inited = false;
public:
	RInitable();
	void setInited(bool i);
	bool isInited() const;
	virtual void init();
	virtual void release() = 0;
	virtual ~RInitable();
};
#endif
