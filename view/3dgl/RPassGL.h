#ifndef RPASSGL_H
#define RPASSGL_H
#include "../GL.h"
#include "RPassDesc.h"
class RDrawPassGL : public RPass
{
private:
	RPassDesc _desc;
	uint _depth_buffer_pointer = 0;
	uint _framebuffer_id = 0;
	uint N;
	std::unique_ptr< uint[] > __texture_pointer_array;
public:
	void init( const RPassDesc &desc ) override
	{
		if( isInited() ) return;
		setInited( true );
		_desc = desc;
		N = desc._buffer_count;
		glGenFramebuffers( 1 , &_framebuffer_id );
		glBindFramebuffer( GL_FRAMEBUFFER , _framebuffer_id );
		int _gl_type;
		int _gl_store;
		int _gl_depth;
		int _gl_format = GL_RGBA;
		switch ( desc._store_type )
		{
			case RBufferStoreType::RBUFFER_BYTE:
				_gl_type = GL_RGBA8;
				_gl_store = GL_UNSIGNED_BYTE;
				_gl_depth = GL_DEPTH_COMPONENT16;
			break;
			case RBufferStoreType::RBUFFER_FLOAT:
				_gl_type = GL_RGBA32F;
				_gl_store = GL_FLOAT;
				_gl_depth = GL_DEPTH_COMPONENT32F;
			break;
			case RBufferStoreType::RBUFFER_INT:
				_gl_type = GL_RGBA32UI;
				_gl_store = GL_UNSIGNED_INT;
				_gl_depth = GL_DEPTH_COMPONENT32F;//
				_gl_format = GL_RGBA_INTEGER;
			break;
		}
		if( N > 0 )
		{
			std::unique_ptr< GLenum[] > DrawBuffers( new GLenum[N] );
			for ( int i = 0; i < N; i++ )
				DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
			glDrawBuffers( N , DrawBuffers.get() );
			__texture_pointer_array = std::move( std::unique_ptr< uint[] >( new uint[N] ) );
			glGenTextures( N , __texture_pointer_array.get() );
			ito( N )
			{
				if( desc._cubemap )
				{
					glBindTexture( GL_TEXTURE_CUBE_MAP , __texture_pointer_array[i] );
					glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
					glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
					glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
					glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
					glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
					for ( int j = 0; j < 6; j++ )
					{
						glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + j , 0 , _gl_type , _desc._size._w , _desc._size._h , 0 , _gl_format , _gl_store , 0 );
					}
					///glFramebufferTexture( GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 + i , __texture_pointer_array[i] , 0 );
					glFramebufferTexture2D( GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 + i , GL_TEXTURE_CUBE_MAP_POSITIVE_X , __texture_pointer_array[i] , 0 );
				}else
				{
					glBindTexture( GL_TEXTURE_2D , __texture_pointer_array[i] );
					glTexImage2D( GL_TEXTURE_2D , 0 , _gl_type , _desc._size._w , _desc._size._h , 0 , _gl_format , _gl_store , 0 );
					glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
					glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
					glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT );
					glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT );
					glFramebufferTexture2D( GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 + i , GL_TEXTURE_2D , __texture_pointer_array[i] , 0 );
				}
			}
		}
		if( desc._foreight_depth_buffer != -1 )
		{
			_depth_buffer_pointer = desc._foreight_depth_buffer;
			if( desc._depth_buffer_access )
				glFramebufferTexture( GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , _depth_buffer_pointer , 0 );
			else
				glFramebufferRenderbuffer( GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT ,
										 GL_RENDERBUFFER , _depth_buffer_pointer );
		}else
		{
			if( desc._cubemap )
			{
				if( desc._depth_buffer_access )
				{
					glGenTextures( 1 , &_depth_buffer_pointer );
					glBindTexture( GL_TEXTURE_CUBE_MAP , _depth_buffer_pointer );
					glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
					glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
					glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
					glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
					glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
					for ( int i = 0; i < 6; i++ )
					{
						glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i , 0 , _gl_depth ,
									  _desc._size._w , _desc._size._h , 0, GL_DEPTH_COMPONENT , _gl_store , 0 );
					}
					glFramebufferTexture( GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , _depth_buffer_pointer , 0 );
				}else
				{
					glGenRenderbuffers( 1 , &_depth_buffer_pointer );
					glBindRenderbuffer( GL_RENDERBUFFER , _depth_buffer_pointer );
					glRenderbufferStorage( GL_RENDERBUFFER , _gl_depth , _desc._size._w , _desc._size._h );
					glFramebufferRenderbuffer( GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER , _depth_buffer_pointer );
				}
			}else
			{
				if( desc._depth_buffer_access )
				{
					glGenTextures( 1 , &_depth_buffer_pointer );
					glBindTexture( GL_TEXTURE_2D , _depth_buffer_pointer );
					glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
					glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
					glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE );
					glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE );
					glTexImage2D( GL_TEXTURE_2D , 0 , _gl_depth ,
								  _desc._size._w , _desc._size._h , 0, GL_DEPTH_COMPONENT , _gl_store , 0 );
					glFramebufferTexture( GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , _depth_buffer_pointer , 0 );
				}else
				{
					glGenRenderbuffers( 1 , &_depth_buffer_pointer );
					glBindRenderbuffer( GL_RENDERBUFFER , _depth_buffer_pointer );
					glRenderbufferStorage( GL_RENDERBUFFER , _gl_depth , _desc._size._w , _desc._size._h );
					glFramebufferRenderbuffer( GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER , _depth_buffer_pointer );
				}
			}
		}
	}
	ViewTypes::RDrawBufferPTR getBufferPtr( int i ) const override
	{
		if( N == 0 )
			return 0;
		return __texture_pointer_array[i];
	}
	uint getBufferCount() override
	{
		return N;
	}
	void clear( bool cd = true ) override
	{
		glBindFramebuffer( GL_FRAMEBUFFER , _framebuffer_id );
		glViewport( 0 , 0 , _desc._size._w , _desc._size._h );
		//if( _desc._store_type == RBufferStoreType::RBUFFER_INT )
		//	glClearColorIuiEXT( 100 , 0 , 0 , 0 );
		//else
		glClearColor( 0.0f , 0.0f , 0.0f , 1.0f );
		if( cd )
		{
			glClearDepth( 1.0f );
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		}
		else
			glClear( GL_COLOR_BUFFER_BIT );
	}
	void bind() const override
	{
		glBindFramebuffer( GL_FRAMEBUFFER , _framebuffer_id );
		glViewport( 0 , 0 , _desc._size._w , _desc._size._h );
	}
	ViewTypes::RDrawBufferPTR getDepthBufferPtr() const override
	{
		return _depth_buffer_pointer;
	}
	void release() override
	{
		if( !isInited() ) return;
		setInited( false );
		glDeleteTextures( N , __texture_pointer_array.get() );
		if( _desc._depth_buffer_access )
			glDeleteTextures( 1 , &_depth_buffer_pointer );
		else
			glDeleteRenderbuffers( 1 , &_depth_buffer_pointer );
		__texture_pointer_array.reset();
	}
	void update( const RSize &size ) override
	{
		if( _desc._size._w == size._w && _desc._size._h == size._h )
			return;
		_desc._size = size;
		release();
		init( _desc );
	}
};
#endif // RPASSGL_H
