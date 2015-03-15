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
	float		_nearplane = 0.1f;
	float		_farplane = 1000.0f;
	f4x4 _view_proj_matrix = f4x4( 1.0f );
public:
	mutable float		_fovx = 1.4f;
	mutable float		_fovy = 1.4f;
	f4x4 const &getViewProj() const;
	f4x4 const &calc();
	void lookAt( const f3 &pos , const f3 &whrlook , const f3 &v3local_y );
	void pos( const f3 &pos );
	void angle( const float phi , const float theta );
	void setAspect( float ax , float ay ) const;
	void perspective( const float nearp , const float farplane , const float aspectx , const float aspecty );
	static f4x4 perpLookUp1x1( const f3 & , const f3 & , const f3 & );
	static f4x4 orthographic( const f3 & , const f3 & , const f3 & , float );
	static void genCubeCamera( f4x4 *out , const f3 &pos );
	bool fristrum( f3 const &p , float size ) const;
};
#endif // RCAMERA_H
