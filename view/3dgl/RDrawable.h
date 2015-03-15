#ifndef __RDRAWABLES_H__
#define __RDRAWABLES_H__
#include "../../base/RBase.h"
#include "RTextures.h"
#include "RAnim.h"
#include <memory>
#include <list>
enum RDrawableType
{
	RDRAWBL_POLYMESH = 0 , RDRAWBL_QUAD = 1 , RDRAWBL_BOX = 2
};
class RSpriteEffect
{
public:
	enum REffectType
	{
		REFFECT_NONE = -1 , REFFECT_GLOW = 0 , REFFECT_FIRE = 1
	};
	int _type;
	float _time = 0.0f;
	float _speed = 0.0;
	float _size = 1.0f;
	bool _dead = false;
	f3 _v3pos = f3( 0.0f , 0.0f , 0.0f );
	f3 _v3dir = f3( 0.0f , 0.0f , 0.0f );
	RSpriteEffect( int type = REFFECT_NONE ) : _type( type ){}
	inline void update( const float dt )
	{
		_time += _speed * dt;
		if( _time > 1.0f ) _dead = true;
	}
};
class RPolymesh
{
public:
	struct RPolyMeshType{
		static const int RBONED_PMESH = 0  , RSTATIC_PMESH = 1;
	};
	int				_flags;
	int				_type;
	f3				_v3size;
	uint        _vertex_count;
	uint        _face_count;
	uint        _bone_count;
	std::unique_ptr< char[] > __vertices;
	std::unique_ptr< unsigned short[] > __indeces;
	std::unique_ptr< RAnimationset[] > __mat4anim;
	uint _anim_count = 0;
	std::unique_ptr< RImage[] > _textures;
	uint _texture_count = 0;
	void release()
	{
		__vertices.reset();
		__indeces.reset();
		_textures.reset();
		__mat4anim.reset();
	}
};
#endif
