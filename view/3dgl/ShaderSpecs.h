#ifndef SHADERSPECS_H
#define SHADERSPECS_H
#include "../../base/RBase.h"
struct RShaderInTypes
{
	static const int vec1 = 0 , vec2 = 1 , vec3 = 2 , ivec1 = 3 , mat4 = 4 , tex = 5 , cubemap = 6 , vec4 = 7 , none = -1;
};
class ShaderMask
{
public:
	static constexpr int MASK_TEXTURED	= 1 << 0;
	static constexpr int MASK_TEXTURED_DIF	= 1 << 1;
	static constexpr int MASK_TEXTURED_NOR 	= 1 << 2;
	static constexpr int MASK_TEXTURED_SPE	= 1 << 3;
	static constexpr int MASK_ANIMATED	= 1 << 4;
	static constexpr int MASK_OWN_ANIMATED	= 1 << 5;
};
struct RTuple
{
	int _type;
	const void *__data;
	uint _count;
};
/*struct RShaderInput
{
	static const uint MAX_INPUT = 50;
	RTuple __tuple[MAX_INPUT];
	bool _loaded[MAX_INPUT];
	uint __id[MAX_INPUT];
	uint c;
	inline void set( int i , int index , int type , const void *data , uint count = 1 )
	{
		__id[index] = i;
		__tuple[index]._type = type;
		__tuple[index].__data = data;
		__tuple[index]._count = count;
		_loaded[index] = false;
	}
	inline int add( int i , int type , const void *data , uint count = 1 )
	{
		__id[c] = i;
		__tuple[c]._type = type;
		__tuple[c].__data = data;
		__tuple[c]._count = count;
		_loaded[c] = false;
		c++;
		return c - 1;
	}
	inline void reset()
	{
		ito( MAX_INPUT )
		{
			__tuple[i]._type = RShaderInTypes::none;
			__tuple[i].__data = nullptr;
			__tuple[i]._count = 0;
			_loaded[i] = true;
			__id[i] = 0;
		}
		c = 0;
	}
};*/
/*class RGraphicProgramm : public RInitable
{
public:
	virtual void bind( RShaderInput & ) = 0;
};*/
#endif // SHADERSPECS_H
