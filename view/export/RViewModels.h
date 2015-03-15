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
		void check(float time);
	} _event;
	float _speed = 1.0f;
	float _mixspeed = 1.0f;
	void update(const float dt);
	void change(const int i, const float speed, RTimeEvent const &event);
};
typedef struct
{
	ViewTypes::RDrawablePTR view_id;
	f4x4 model;
}
ViewInfo;
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
