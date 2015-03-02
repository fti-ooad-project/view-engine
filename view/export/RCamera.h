#ifndef RCAMERA_H
#define RCAMERA_H
#include "../../linalg/vec.h"
#include "../../linalg/mat.h"
#include <memory>
#include "../../base/RModels.h"
#include "../../base/RBase.h"
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
	static f4x4 perpLookUp1x1( const f3 &pos, const f3 &look, const f3 &up )
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

		f3 x = -vecx( look, up );
		f3 y = up;
		view = f4x4(
				   x.x(), y.x(), look.x(), 0.0f,
				   x.y(), y.y(), look.y(), 0.0f,
				   x.z(), y.z(), look.z(), 0.0f,
				   -x * pos, -y * pos, -look * pos, 1.0f );
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
#endif // RCAMERA_H
