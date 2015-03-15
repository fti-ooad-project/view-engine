#include "../base/REventer.h"
KeyStates::KeyStates(const keystate *cs, const keystate *ls) :
__cur_states(cs)
, __last_states(ls)
{}
MouseStates::MouseStates(const keystate *cs, const keystate *ls, const f2 &cp, const f2 &lp) :
__cur_states(cs)
,__last_states(ls)
, __cur_pos(cp)
, __last_pos(lp)
{}
REventer::REventer()
{
	ito( 3 )
		_cur[i] = 0;
	ito( 3 )
		_last[i] = 1;
	ito( 2 )
		memset(__key_state[i] , 0 , MAX_KEY * sizeof(keystate ));
	ito(2)
		memset(__mouse_state[i], 0, 3 * sizeof(keystate));
	ito(2)
		memset(__mouse_pos, 0, 2*sizeof(f2));
}
void REventer::addMouseFunc(MouseFunc func)
{
	_mouse_func.push_back(func);
}
void REventer::addKeyFunc(KeyFunc func)
{
	_key_func.push_back(func);
}
void REventer::addTimeFunc(TimeFunc func)
{
	_time_func.push_back(func);
}
void REventer::update(const keystate *in_keys, const keystate *in_mous, const f2 *mp)
{
	if (in_keys != nullptr)
	{
		_last[0] = _cur[0];
		_cur[0] = (_cur[0] + 1) & 1;
		memcpy(__key_state[_cur[0]], in_keys, MAX_KEY * sizeof(keystate));
	}
	if (in_mous != nullptr)
	{
		_last[1] = _cur[1];
		_cur[1] = (_cur[1] + 1) & 1;
		memcpy(__mouse_state[_cur[1]], in_mous, 3 * sizeof(keystate));
	}
	if (mp != nullptr)
	{
		_last[2] = _cur[2];
		_cur[2] = (_cur[2] + 1) & 1;
		memcpy(&__mouse_pos[_cur[2]], mp, sizeof(f2));
	}
}
void REventer::call()
{
	updateTime();
	KeyStates ks{ __key_state[_cur[0]] , __key_state[_last[0]] };
	MouseStates ms{ __mouse_state[_cur[1]] , __mouse_state[_last[1]] , __mouse_pos[_cur[2]] , __mouse_pos[_last[2]] };
	for (MouseFunc i : _mouse_func)
		i(ms, _dt);
	for (KeyFunc i : _key_func)
		i(ks, _dt);
	for (TimeFunc i : _time_func)
		i(_dt);
}
void REventer::release()
{
	_key_func.clear();
	_mouse_func.clear();
	_time_func.clear();
}
REventer::~REventer()
{
	release();
}