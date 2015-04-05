#include "../view/export/RViewModels.h"
void AnimationMixer::update( const float dt )
{
	if( _moment._mixing )
	{
		_moment._moment += dt * _mixspeed;
		if( _moment._moment > 1.0f )
		{
			_moment._moment = 0.0f;
			_moment._mixing = false;
		}
	} else
	{
		_moment._moment += dt * _speed;
		if( _moment._moment > 1.0f )
		{
			_moment._moment = _moment._moment - floorf( _moment._moment );
		}
		_event.check( _moment._moment );
	}
}
void AnimationMixer::change( const int i , const float speed , RTimeEvent const &event )
{
	_moment._mixing = true;
	_moment._last_moment = _moment._moment;
	_moment._moment = 0.0f;
	_moment._last_set = _moment._cur_set;
	_moment._cur_set = i;
	_speed = speed;
	_event = event;
}
void AnimationMixer::RTimeEvent::check( float time )
{
	if( !_active ) return;
	if( time >= _switch_time )
	{
		_func();
	}
}

void ParticleEffect::update( float dt )
{
	_time += dt * _speed;
}
