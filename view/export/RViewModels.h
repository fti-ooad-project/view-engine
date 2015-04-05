#ifndef __RViewModels_H__
#define __RViewModels_H__
//#include "../../hpp/linalg/RLinAlg.hpp"
#include "../../linalg/vec.h"
#include "../../linalg/mat.h"
#include "../../base/RBase.h"
#include <functional>
/*started 29.03.14
*@schreiner
*/
class ViewTypes
{
public:
	typedef uint RDrawBufferPTR;
	typedef uint RDrawablePTR;
	typedef uint RLightSourcePTR;
	typedef uint RDrawInstancePTR;
	typedef uint RBoneAnimInTexPTR;
};
class RLightSourceType
{
public:
	static const int RLIGHT_OMNI = 0 , RLIGHT_DIRECT = 1;
};
class RLightSourceShape
{
public:
	static const int RLIGHTSHAPE_CIRCLE = 0 , RLIGHTSHAPE_CIRCLE_SMOOTH = 1;
};
struct AnimationMixer
{
	struct RMoment
	{
		float _moment = 0.0f;
		int _cur_set = 0;
		int _last_set = 0;
		float _last_moment = 0.0f;
		bool _mixing = false;
	} _moment;
	struct RTimeEvent
	{
		bool _active = false;
		std::function< void( void ) > _func;
		float _switch_time;
		void check( float );
	} _event;
	float _speed = 1.0f;
	float _mixspeed = 1.0f;
	void update( const float );
	void change( const int , const float , RTimeEvent const &event );
};
class UnitInstanceState
{
public:
	std::vector< ViewTypes::RDrawablePTR > _view;
	f3 _pos;
	f3 _look;
	f3 _up;
	bool _auto_height;
	int selectid;
	mutable AnimationMixer _animstat;
};
struct LightState
{
	bool _cast_shadow;
	int _type;
	f3	_pos;
	f3	_dir;
	f4	_colori;
	float _size;
};
struct ParticleEffect
{
	f3 _pos;
	f3 _dir;
	float _radius;
	float _time;
	float _speed;
	float _depth;
	uint _particle_count;
	int _view_id;
	void update( float );
};
#endif /* __RViewModels_H__ */
