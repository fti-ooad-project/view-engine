#ifndef __RLIGHT_H__
#define __RLIGHT_H__
#include "RPassDesc.h"
class RLightSourceType
{
	static const int RLIGHT_OMNI = 0 , RLIGHT_DIRECT = 1;
};
class RLightSourceShape
{
	static const int RLIGHTSHAPE_CIRCLE = 0 , RLIGHTSHAPE_CIRCLE_SMOOTH = 1;
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
#endif /* __RLIGHT_H__ */
