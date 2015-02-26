#ifndef RANIM_H
#define RANIM_H
#include "base/RBase.h"
#include "view/3dgl/RViewModels.h"
class RAnimationset
{
public:
	uint        _frame_count;
	uint        _bone_count;
	std::unique_ptr< f4x4[] > __data;
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
#endif // RANIM_H
