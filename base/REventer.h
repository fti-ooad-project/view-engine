#ifndef REVENTER_H
#define REVENTER_H
#include "RBase.h"
typedef bool keystate;
struct	KeyStates
{
	const keystate * __cur_states;
	const keystate * __last_states;
	KeyStates( const keystate *cs , const keystate *ls ): __cur_states( cs ) , __last_states( ls ){}
};
struct	MouseStates
{
	const keystate *__cur_states;
	const keystate *__last_states;
	const f2 __cur_pos;
	const f2 __last_pos;
	MouseStates( const keystate *cs , const keystate *ls , const f2 &cp , const f2 &lp ):
	__cur_states( cs ) , __last_states( ls ) , __cur_pos( cp ) , __last_pos( lp ){}
};
typedef std::function< void( const KeyStates& , const float ) > KeyFunc;
typedef std::function< void( const MouseStates& , const float ) > MouseFunc;
typedef std::function< void( const float ) > TimeFunc;
class REventer : public RInitable , public RTimer
{
private:
	std::vector< KeyFunc > _key_func;
	std::vector< MouseFunc > _mouse_func;
	std::vector< TimeFunc > _time_func;
	keystate    __key_state[2][MAX_KEY];
	keystate	__mouse_state[2][3];
	f2			__mouse_pos[2];
	int         _cur[3] = {0} , _last[3] = {1};
public:
	inline void addMouseFunc( MouseFunc func )
	{
		_mouse_func.push_back( func );
	}
	inline void addKeyFunc( KeyFunc func )
	{
		_key_func.push_back( func );
	}
	inline void addTimeFunc( TimeFunc func )
	{
		_time_func.push_back( func );
	}
	inline void update( const keystate *in_keys , const keystate *in_mous , const f2 *mp )
	{
		if( in_keys != nullptr )
		{
			_last[0] = _cur[0];
			_cur[0] = ( _cur[0] + 1 ) & 1;
			memcpy( __key_state[_cur[0]] , in_keys , MAX_KEY * sizeof( keystate ) );
		}
		if( in_mous != nullptr )
		{
			_last[1] = _cur[1];
			_cur[1] = ( _cur[1] + 1 ) & 1;
			memcpy( __mouse_state[_cur[1]] , in_mous , 3 * sizeof( keystate ) );
		}
		if( mp != nullptr )
		{
			_last[2] = _cur[2];
			_cur[2] = ( _cur[2] + 1 ) & 1;
			memcpy( &__mouse_pos[_cur[2]] , mp , sizeof( f2 ) );
		}
	}
	void call()
	{
		updateTime();
		KeyStates ks{ __key_state[_cur[0]] , __key_state[_last[0]] };
		MouseStates ms{ __mouse_state[_cur[1]] , __mouse_state[_last[1]] , __mouse_pos[_cur[2]] , __mouse_pos[_last[2]] };
		for( MouseFunc i: _mouse_func )
			i( ms , _dt );
		for( KeyFunc i: _key_func )
			i( ks , _dt );
		for( TimeFunc i: _time_func )
			i( _dt );
	}
	void release()
	{
		_key_func.clear();
		_mouse_func.clear();
		_time_func.clear();
	}
	~REventer()
	{
		release();
	}
};
#endif // REVENTER_H
