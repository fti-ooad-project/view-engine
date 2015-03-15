#include "../view/export/RViewModels.h"
void RAnimationMixer::update(const float dt)
{
	if (_moment._mix)
	{
		_moment._time += dt * _mixspeed;
		if (_moment._time > 1.0f)
		{
			_moment._time = 0.0f;
			_moment._mix = false;
		}
	}
	else
	{
		_moment._time += dt * _speed;
		if (_moment._time > 1.0f)
		{
			_moment._time = 0.0f;
		}
		_event.check(_moment._time);
	}
}
void RAnimationMixer::change(const int i, const float speed, RTimeEvent const &event)
{
	_moment._mix = true;
	_moment._last_time = _moment._time;
	_moment._time = 0.0f;
	_moment._last_set = _moment._cur_set;
	_moment._cur_set = i;
	_speed = speed;
	_event = event;
}
void RAnimationMixer::RTimeEvent::check(float time)
{
	if (!_active) return;
	if (time >= _time)
	{
		_func();
	}
}