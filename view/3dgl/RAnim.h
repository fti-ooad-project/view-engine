#ifndef RANIM_H
#define RANIM_H
#include "base/RBase.h"
class RAnimationset
{
public:
	uint        _frame_count;
	uint        _bone_count;
	std::unique_ptr< f4x4[] > __data;
	RAnimationset() = default;
	RAnimationset( std::unique_ptr< f4x4[] > &&data , uint frame_count , uint bone_count ):
	_frame_count( frame_count )
	, _bone_count( bone_count )
	, __data( std::move( data ) )
	{
	}
	RAnimationset( RAnimationset &&a ):
	_frame_count( a._frame_count )
	, _bone_count( a._bone_count )
	, __data( std::move( a.__data ) )
	{
	}
	void operator=( RAnimationset &&a )
	{
		_frame_count = a._frame_count;
		_bone_count = a._bone_count;
		__data = std::move( a.__data );
	}
};
#endif // RANIM_H
