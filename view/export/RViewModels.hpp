#ifndef __RViewModels_H__
#define __RViewModels_H__
//#include "../../hpp/linalg/RLinAlg.hpp"
#include "../../linalg/vec.h"
#include "../../linalg/mat.h"
#include "../../base/RBase.h"
/*started 29.03.14
*@schreiner
*/
class ViewTypes
{
public:
	static const int CREATE_NEW = -1;
	typedef uint RDrawBufferPTR;
	typedef uint RDrawablePTR;
	typedef uint RLightSourcePTR;
	typedef uint RDrawInstancePTR;
	typedef uint RBoneAnimInTexPTR;
};
struct RAnimationMixer
{
	struct RMoment
	{
		float _time = 0.0f;
		int _cur_set = 0;
		int _last_set = 0;
		float _last_time = 0.0f;
		bool _mix = false;
	} _moment;
	struct RTimeEvent
	{
		bool _active = false;
		std::function< void( void ) > _func;
		float _time;
		void check( float time )
		{
			if( !_active ) return;
			if( time >= _time )
			{
				_func();
			}
		}
	} _event;
	float _speed = 1.0f;
	float _mixspeed = 1.0f;
	void update( const float dt )
	{
		if( _moment._mix )
		{
			_moment._time += dt * _mixspeed;
			if( _moment._time > 1.0f )
			{
				_moment._time = 0.0f;
				_moment._mix  = false;
			}
		}
		else
		{
			_moment._time += dt * _speed;
			if( _moment._time > 1.0f )
			{
				_moment._time = 0.0f;
			}
			_event.check( _moment._time );
		}
	}
	void change( const int i , const float speed , RTimeEvent const &event )
	{
		_moment._mix = true;
		_moment._last_time = _moment._time;
		_moment._time = 0.0f;
		_moment._last_set = _moment._cur_set;
		_moment._cur_set = i;
		_speed = speed;
		_event = event;
	}
};
typedef std::tuple< ViewTypes::RDrawablePTR , f4x4 > ViewInfo;
class RDrawableState
{
public:
	std::vector< ViewInfo > _view;
	RAnimationMixer _animstat;
};
struct RLightState
{
	bool _cast_shadow;
	int _type;
	f3	_pos;
	f3	_dir;
	f4	_colori;
	float _size;
	int _shape;
};
#endif /* __RViewModels_H__ */
