#ifndef __ROPENGLINTFC_H__
#define __ROPENGLINTFC_H__
//#include "RLinAlg.hpp"
#include "RViewModels.h"
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include "../GL.h"
#include "../../base/RFileParser.hpp"
#include "../../base/RFileloader.h"
struct RAnimPref
{
    int _bone_count;
    float _speed;
    int _type;
    int _frame_count;
};
class RGraphicProgrammGL : public RGraphicProgramm
{
private:
	uint _vertex_program , _fragment_program , _m_program = 0 , _geometric_program = 0 , _tess_program = 0 , _tess_est_program = 0;
public:
	void init( std::string frag_file , std::string vert_file , std::string geom_file = "" , std::string tess_file = "" , std::string tessest_file = "" ){
#ifdef RLOG
			LOG << "_________________________\n" << frag_file << "_\n" << vert_file << "_\n" << geom_file << "_\n" << tess_file << "_\n" << tessest_file << "_\nis processed\n";
#endif
		try
		{
			auto proc = []( uint type , std::string const &raw )
			{
				int out = glCreateShader( type );
				std::string src = RFileParser::genParsed( raw );
				GLint l = src.length();
				char const *temp = src.c_str();
				glShaderSource( out , 1 , &temp , &l );
				glCompileShader( out );
				GLint compiled;
				glGetShaderiv( out , GL_COMPILE_STATUS , &compiled );
				if ( !compiled )
				{
					GLint length;
					glGetShaderiv( out , GL_INFO_LOG_LENGTH , &length );
					std::string log( length , ' ' );
					glGetShaderInfoLog( out , length , &length , &log[0] );
					std::stringstream temp( src );
					std::string t;
					int i = 1;
					while( std::getline( temp , t ) )
					{
						LOG << i << ":" << t << "\n";
						i++;
					}
					printf( "Shader Log Data: %s\n" , log.data() );
					throw std::logic_error( "shader compilation error" );
				}
				return out;
			};
			_vertex_program = proc( GL_VERTEX_SHADER , vert_file );
			_fragment_program = proc( GL_FRAGMENT_SHADER , frag_file );
			if( geom_file.size() > 1 ) _geometric_program = proc( GL_GEOMETRY_SHADER , geom_file );
			if( tess_file.size() > 1 ) _tess_program = proc( GL_TESS_CONTROL_SHADER , tess_file );
			if( tessest_file.size() > 1 ) _tess_est_program = proc( GL_TESS_EVALUATION_SHADER , tessest_file );
			_m_program = glCreateProgram();
			glAttachShader( _m_program , _vertex_program );
			glAttachShader( _m_program , _fragment_program );
			if( _geometric_program ) glAttachShader( _m_program , _geometric_program );
			if( _tess_program ) glAttachShader( _m_program , _tess_program );
			if( _tess_est_program ) glAttachShader( _m_program , _tess_est_program );
			glLinkProgram( _m_program );
			GLint compiled;
			glGetProgramiv( _m_program , GL_LINK_STATUS , &compiled );
			if ( !compiled )
			{
				GLint length;
				glGetProgramiv( _m_program , GL_INFO_LOG_LENGTH , &length );
				std::string log( length , ' ' );
				glGetProgramInfoLog( _m_program , length , &length , &log[0] );
#ifdef RLOG
				LOG << "shader link error: " << log << "\n";
#endif
				throw std::logic_error( "shader linkage error" );
				return;
			}
#ifdef RLOG
			LOG << "have been processed\n_______\n";
#endif
		}catch( std::logic_error &err )
		{
			LOG << "shader compile exception: " << err.what() << "\n";
		}
	}
	inline void bind( RShaderInput &in )
    {
		glUseProgram( _m_program );
		uint tc = 0;///five becouse polymeshes bind textures from 0 to 5( diffuse, spec , normals... )
		for( uint i = 0; i < RShaderInput::MAX_INPUT; ++i )
        {
            if( in._loaded[i] ) continue;
            in._loaded[i] = true;
			setUniform( in.__tuple[i] , in.__id[i] , tc );
		}
    }
	void release()
    {
		if( !_m_program ) glDeleteProgram( _m_program );
		if( !_vertex_program ) glDeleteShader( _vertex_program );
		if( !_fragment_program ) glDeleteShader( _fragment_program );
		if( !_geometric_program ) glDeleteShader( _geometric_program );
		if( !_tess_program ) glDeleteShader( _tess_program );
		if( !_tess_est_program ) glDeleteShader( _tess_est_program );
    }
	static void setUniform( RTuple &tuple , uint i , uint &tc )
	{
		switch ( tuple._type )
		{
			case RShaderInTypes::vec1:
				glUniform1fv( i , tuple._count , static_cast< const float* >( tuple.__data ) );
			break;
			case RShaderInTypes::vec2:
				glUniform2fv( i , tuple._count , static_cast< const float* >( tuple.__data ) );
			break;
			case RShaderInTypes::vec3:
				glUniform3fv( i , tuple._count , static_cast< const float* >( tuple.__data ) );
			break;
			case RShaderInTypes::vec4:
				glUniform4fv( i , tuple._count , static_cast< const float* >( tuple.__data ) );
			break;
			case RShaderInTypes::ivec1:
				glUniform1iv( i , tuple._count , static_cast< const int* >( tuple.__data ) );
			break;
			case RShaderInTypes::mat4:
				glUniformMatrix4fv( i , tuple._count , GL_FALSE , static_cast< const float* >( tuple.__data ) );
			break;
			case RShaderInTypes::tex:
				glActiveTexture( GL_TEXTURE0 + tc );
				glBindTexture( GL_TEXTURE_2D , *static_cast< const uint* >( tuple.__data ) );
				glUniform1i( i , tc );
				++tc;
			break;
			case RShaderInTypes::cubemap:
				glActiveTexture( GL_TEXTURE0 + tc );
				glBindTexture( GL_TEXTURE_CUBE_MAP , *static_cast< const uint* >( tuple.__data ) );
				glUniform1i( i , tc );
				++tc;
			break;
		}
	}
	~RGraphicProgrammGL()
	{
		release();
	}
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
