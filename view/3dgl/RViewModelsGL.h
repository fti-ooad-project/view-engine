#ifndef __ROPENGLINTFC_H__
#define __ROPENGLINTFC_H__
//#include "RLinAlg.hpp"
#include "ShaderSpecs.h"
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "../../base/RFileParser.h"
#include "../../base/RFileloader.h"
struct RAnimPref
{
    int _bone_count;
    float _speed;
    int _type;
    int _frame_count;
};
class RGraphicProgrammGL
{
private:
	uint _vertex_program , _fragment_program , _m_program = 0 , _geometric_program = 0 , _tess_program = 0 , _tess_est_program = 0;
public:
	void init(std::string frag_file, std::string vert_file, std::string geom_file = "", std::string tess_file = "", std::string tessest_file = "");
	//inline void bind(RShaderInput &in);
	void bind();
	void release();
	static void setUniform(RTuple &tuple, uint i, uint &tc);
	~RGraphicProgrammGL();
};
/*class RPostProcessBlurGL : public RPostProcess
{
private:
	mutable RGraphicProgrammGL _prog;
	mutable RShaderInput _in;
	RPolyQuadGL _quad;
	mutable RScenePassGL __pass[2];
public:
	virtual void init()
	{
		_prog.init( "res/shaders/glsl/blur.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" );
		ito( 2 )
			__pass[i].init( RPassDesc( 512 , 512 , 1 , -1 , RGL::RBufferType::RBUFFER_FLOAT ) );
		_quad.init();
	}
	virtual ViewTypes::RDrawBufferPTR process( const ViewTypes::RDrawBufferPTR buf , const uint w , const uint h , const uint o  )
	{
		ito( 2 )
		{
			__pass[i].update( RPassDesc( w , h , 1 , -1 , RGL::RBufferType::RBUFFER_FLOAT ) );
			__pass[i].clear();
		}
		_in.reset();
		int i = _in.add( 0 , RShaderInTypes::tex , static_cast< const void* >( buf ) );
		const int iter = 40;
		const float radius = 0.1f;
		f2 v( 1.0f , 0.0f );
		_in.add( 10 , RShaderInTypes::vec1 , static_cast< const void* >( &radius ) );
		_in.add( 11 , RShaderInTypes::ivec1 , static_cast< const void* >( &iter ) );
		int vi = _in.add( 1 , RShaderInTypes::vec2 , static_cast< const void* >( &v ) );
		__pass[0].bind();
		_prog.bind( _in );
		_quad.draw();
		v = f2( 0.0 , 1.0 );
		_in.set( 0 , i , RShaderInTypes::tex , static_cast< const void* >( __pass[0].getBufferPtr() ) );
		_in.set( 1 , vi , RShaderInTypes::vec2 , static_cast< const void* >( &v ) );
		__pass[1].bind();
		_prog.bind( _in );
		_quad.draw();
		return __pass[1].getBufferPtr();
	}
	virtual void release()
	{
		_prog.release();
		_quad.release();
		ito( 2 )
			__pass[i].release();
	}
	~RPostProcessBlurGL()
	{
		release();
	}
};*/
#endif
