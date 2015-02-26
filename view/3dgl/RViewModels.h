#ifndef __RViewModels_H__
#define __RViewModels_H__
//#include "../../hpp/linalg/RLinAlg.hpp"
#include "linalg/vec.h"
#include "linalg/mat.h"
#include "RVertex.h"
#include <memory>
#include "base/RModels.h"
#include "base/RBase.h"
/*started 29.03.14
*@schreiner
*/
class ShaderMask
{
public:
	static constexpr int MASK_TEXTURED	= 1 << 0;
	static constexpr int MASK_TEXTURED_DIF 	= 1 << 1;
	static constexpr int MASK_TEXTURED_NOR 	= 1 << 2;
	static constexpr int MASK_TEXTURED_SPE	= 1 << 3;
	static constexpr int MASK_ANIMATED	= 1 << 4;
};
struct RSize
{
	uint _w , _h;
	RSize( uint w = 0 , uint h = 0 ): _w( w ) , _h( h ) {}
	RSize( const RSize &size ): _w( size._w ) , _h( size._h ) {}
};
class ViewTypes
{
public:
	static const int CREATE_NEW = -1;
	typedef uint RDrawBufferPTR;
	typedef uint RDrawablePTR;
	typedef uint RLightSourcePTR;
	typedef uint RDrawInstancePTR;
	typedef uint RBoneAnimInTexPTR;
};
class RCamera : public REntity
{
private:
	mutable float		_fovx = 1.4f;
	mutable float		_fovy = 1.4f;
	float		_nearplane = 0.1f;
    float		_farplane = 1000.0f;
	f4x4 _view_proj_matrix = f4x4( 1.0f );
public:
	f4x4 const &getViewProj() const
    {
        return _view_proj_matrix;
    }
	f4x4 const &calc()
	{
		if( _calc_m4x4 ) return _view_proj_matrix;
		_calc_m4x4 = false;
		float    h, w, Q;
		w = ( float )1.0f / tan( _fovx * 0.5 );
		h = ( float )1.0f / tan( _fovy * 0.5 );
		Q = _farplane / ( _farplane - _nearplane );
		f4x4 proj_matrix( 0.0f );

		proj_matrix( 0 , 0 ) = w;
		proj_matrix( 1 , 1 ) = h;
		proj_matrix( 2 , 2 ) = Q;
		proj_matrix( 3 , 2 ) = -Q * _nearplane;
		proj_matrix( 2 , 3 ) = 1.0f;
		f3 const &z = _v3local_z;
		f3 const &x = _v3local_x;
		f3 const &y = _v3local_y;
		f4x4 const view_matrix = f4x4(
						  x.x() , y.x() , z.x() , 0.0f,
						  x.y() , y.y() , z.y() , 0.0f,
						  x.z() , y.z() , z.z() , 0.0f,
						  -x * _v3pos, -y * _v3pos, -z * _v3pos, 1.0f );
		_view_proj_matrix =  view_matrix * proj_matrix;
		//_view_proj_matrix.print();
		//_view_proj_matrix.row(0).print();
		return _view_proj_matrix;
	}
	void lookAt( const f3 &pos , const f3 &whrlook , const f3 &v3local_y )
	{
		_v3local_z = ( whrlook - pos ).g_norm();
		_v3local_x = vecx( _v3local_z , v3local_y ).g_norm();
		_v3local_y = vecx( _v3local_x , _v3local_z );
		_v3pos = pos;
		this->pushChange();
	}
	void pos( const f3 &pos )
	{
		if( pos.g_dist2( _v3pos ) < 0.001f ) return;
		_v3pos = pos;
		this->pushChange();
	}
	void angle( const float phi , const float theta )
	{
		//_v3rotation += f3( phi , theta , 0.0f );
		_v3local_z = f3( sin( theta ) * cos( phi ) , sin( theta ) * sin( phi ) , cos( theta ) );
		_v3local_x = vecx( _v3local_z , f3( 0.0f , 0.0f , 1.0f ) ).g_norm();
		_v3local_y = vecx( _v3local_x , _v3local_z );
		this->pushChange();
	}
	void setAspect( float ax , float ay ) const
	{
		_fovx = ax;
		_fovy = ay;
		pushChange();
	}
	void perspective( const float nearp, const float farplane, const float aspectx, const float aspecty )
    {
        _nearplane = nearp;
        _farplane = farplane;
        _fovx = aspectx;
        _fovy = aspecty;
    }
	static f4x4 perpLookUp1x1( const f3 &pos, const f3 &z, const f3 &v3local_z )
    {
		f4x4 view, proj;
        float farp = 1000.0f, nearp = 1.0f;
		float Q;
		Q = farp / ( farp - nearp );
		proj = f4x4( 0.0f );
		proj( 0, 0 ) = 1.0;
		proj( 1 , 1 ) = 1.0;
		proj( 2 , 2 ) = Q;
		proj( 3 , 2 ) = - Q * nearp;
		proj( 2 , 3 ) = 1.0f;

		f3 x = -vecx( z, v3local_z );
		f3 y = -v3local_z;
        view = f4x4(
				   x.x(), y.x(), z.x(), 0.0f,
				   x.y(), y.y(), z.y(), 0.0f,
				   x.z(), y.z(), z.z(), 0.0f,
                   -x * pos, -y * pos, -z * pos, 1.0f );
		//view.print();
		return view * proj;
    }
	static f4x4 orthographic( const f3 &pos, const f3 &z, const f3 &local_y )
	{
		f4x4 view, proj;
		float farp = 100.0f, nearp = 1.0f;
		float Q = 1.0f / 300.0f;//1.0f / ( farp - nearp );
		proj = f4x4( 0.0f );
		float k = 1.0f / 50.0f;
		proj( 0 , 0 ) = k;
		proj( 1 , 1 ) = k;
		proj( 2 , 2 ) = Q;
		proj( 3 , 3 ) = 1.0f;
		//proj( 0 , 3 ) = 1.0f;
		//proj( 1 , 3 ) = 1.0f;
		//proj( 2 , 3 ) = 1.0f;
		f3 x = vecx( z , local_y ).g_norm();
		f3 y = vecx( x , z ).g_norm();
		view = f4x4(
				   x.x(), y.x(), z.x(), 0.0f,
				   x.y(), y.y(), z.y(), 0.0f,
				   x.z(), y.z(), z.z(), 0.0f,
				   -x * pos, -y * pos, -z * pos, 1.0f );
		return view * proj;
	}
	static void genCubeCamera( f4x4 *out , const f3 &pos )
	{
		out[0] = RCamera::perpLookUp1x1( pos , f3( 1.0f, 0.0f, 0.0f ) , f3( 0.0f, 1.0f, 0.0f ) );
		out[1] = RCamera::perpLookUp1x1( pos , f3( -1.0f, 0.0f, 0.0f ) , f3( 0.0f, 1.0f, 0.0f ) );
		out[2] = RCamera::perpLookUp1x1( pos , f3( 0.0f, 1.0f, 0.0f ) , f3( 0.0f, 0.0f, -1.0f ) );
		out[3] = RCamera::perpLookUp1x1( pos , f3( 0.0f, -1.0f, 0.0f ) , f3( 0.0f, 0.0f, 1.0f ) );
		out[4] = RCamera::perpLookUp1x1( pos , f3( 0.0f, 0.0f, 1.0f ) , f3( 0.0f, 1.0f, 0.0f ) );
		out[5] = RCamera::perpLookUp1x1( pos , f3( 0.0f, 0.0f, -1.0f ) , f3( 0.0f, 1.0f, 0.0f ) );
	}
};
struct RShaderInTypes
{
	static const int vec1 = 0 , vec2 = 1 , vec3 = 2 , ivec1 = 3 , mat4 = 4 , tex = 5 , cubemap = 6 , vec4 = 7 , none = -1;
};
struct RTuple
{
	int _type;
	const void *__data;
	uint _count;
};
struct RShaderInput
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
};
class RGraphicProgramm : public RInitable
{
public:
	virtual void bind( RShaderInput & ) = 0;
};
#endif /* __RViewModels_H__ */
