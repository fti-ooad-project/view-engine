#include "../view/3dgl/RPassGL.h"
#include "../view/GL.h"
void RDrawPassGL::init( const RPassDesc &desc )
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
	int _gl_format;
	switch( desc._store_type )
	{
		case RBufferStoreType::RBUFFER_BYTE:
		{
			switch( desc._component_number )
			{
				case 1:
				{
					_gl_type = GL_R8;
					_gl_format = GL_R;
					_gl_store = GL_UNSIGNED_BYTE;
				}
				break;
				case 2:
				{
					_gl_type = GL_RG8;
					_gl_format = GL_RG;
					_gl_store = GL_UNSIGNED_BYTE;
				}
				break;
				case 3:
				{
					_gl_type = GL_RGB8;
					_gl_format = GL_RGB;
					_gl_store = GL_UNSIGNED_BYTE;
				}
				break;
				case 4:
				{
					_gl_type = GL_RGBA8;
					_gl_format = GL_RGBA;
					_gl_store = GL_UNSIGNED_BYTE;
				}
				break;
			}
			_gl_depth = GL_DEPTH_COMPONENT16;
		}
		break;
		case RBufferStoreType::RBUFFER_FLOAT:
		{
			switch( desc._component_number )
			{
				case 1:
				{
					_gl_format = GL_R;
					_gl_type = GL_R32F;
					_gl_store = GL_FLOAT;
				}
				break;
				case 2:
				{
					_gl_format = GL_RG;
					_gl_type = GL_RG32F;
					_gl_store = GL_FLOAT;
				}
				break;
				case 3:
				{
					_gl_format = GL_RGB;
					_gl_type = GL_RGB32F;
					_gl_store = GL_FLOAT;
				}
				break;
				case 4:
				{
					_gl_format = GL_RGBA;
					_gl_type = GL_RGBA32F;
					_gl_store = GL_FLOAT;
				}
				break;
			}
			_gl_depth = GL_DEPTH_COMPONENT32F;

		}
		break;
		case RBufferStoreType::RBUFFER_INT:
		{
			switch( desc._component_number )
			{
				case 1:
				{
					_gl_type = GL_R32UI;
					_gl_store = GL_UNSIGNED_INT;
					_gl_format = GL_RED_INTEGER;
				}
				break;
				case 2:
				{
					_gl_type = GL_RG32UI;
					_gl_store = GL_UNSIGNED_INT;
					_gl_format = GL_RG_INTEGER;
				}
				break;
				case 3:
				{
					_gl_type = GL_RGB32UI;
					_gl_store = GL_UNSIGNED_INT;
					_gl_format = GL_RGB_INTEGER;
				}
				break;
				case 4:
				{
					_gl_type = GL_RGBA32UI;
					_gl_store = GL_UNSIGNED_INT;
					_gl_format = GL_RGBA_INTEGER;
				}
				break;
			}
			_gl_depth = GL_DEPTH_COMPONENT32F;
		}
		break;
	}
	if( N > 0 )
	{
		std::unique_ptr< GLenum[] > DrawBuffers( new GLenum[ N ] );
		for( int i = 0; i < N; i++ )
			DrawBuffers[ i ] = GL_COLOR_ATTACHMENT0 + i;
		glDrawBuffers( N , DrawBuffers.get() );
		__texture_pointer_array = std::move( std::unique_ptr< uint[] >( new uint[ N ] ) );
		glGenTextures( N , __texture_pointer_array.get() );
		ito( N )
		{
			if( desc._cubemap )
			{
				glBindTexture( GL_TEXTURE_CUBE_MAP , __texture_pointer_array[ i ] );
				glTexParameterf( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
				glTexParameterf( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
				glTexParameterf( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE );
				glTexParameterf( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE );
				glTexParameterf( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_WRAP_R , GL_CLAMP_TO_EDGE );
				for( int j = 0; j < 6; j++ )
				{
					glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + j , 0 , _gl_type , _desc._size._w , _desc._size._h , 0 , _gl_format , _gl_store , 0 );
				}
				///glFramebufferTexture( GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 + i , __texture_pointer_array[i] , 0 );
				glFramebufferTexture2D( GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 + i , GL_TEXTURE_CUBE_MAP_POSITIVE_X , __texture_pointer_array[ i ] , 0 );
			} else
			{
				glBindTexture( GL_TEXTURE_2D , __texture_pointer_array[ i ] );
				glTexImage2D( GL_TEXTURE_2D , 0 , _gl_type , _desc._size._w , _desc._size._h , 0 , _gl_format , _gl_store , 0 );
				glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE );
				glFramebufferTexture2D( GL_FRAMEBUFFER , GL_COLOR_ATTACHMENT0 + i , GL_TEXTURE_2D , __texture_pointer_array[ i ] , 0 );
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
	} else
	{
		if( desc._cubemap )
		{
			if( desc._depth_buffer_access )
			{
				glGenTextures( 1 , &_depth_buffer_pointer );
				glBindTexture( GL_TEXTURE_CUBE_MAP , _depth_buffer_pointer );
				glTexParameterf( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
				glTexParameterf( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
				glTexParameterf( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE );
				glTexParameterf( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE );
				glTexParameterf( GL_TEXTURE_CUBE_MAP , GL_TEXTURE_WRAP_R , GL_CLAMP_TO_EDGE );
				for( int i = 0; i < 6; i++ )
				{
					glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i , 0 , GL_DEPTH_COMPONENT32F ,
								  _desc._size._w , _desc._size._h , 0 , GL_DEPTH_COMPONENT , GL_FLOAT , 0 );
				}
				glFramebufferTexture( GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , _depth_buffer_pointer , 0 );
			} else
			{
				glGenRenderbuffers( 1 , &_depth_buffer_pointer );
				glBindRenderbuffer( GL_RENDERBUFFER , _depth_buffer_pointer );
				glRenderbufferStorage( GL_RENDERBUFFER , _gl_depth , _desc._size._w , _desc._size._h );
				glFramebufferRenderbuffer( GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER , _depth_buffer_pointer );
			}
		} else
		{
			if( desc._depth_buffer_access )
			{
				glGenTextures( 1 , &_depth_buffer_pointer );
				glBindTexture( GL_TEXTURE_2D , _depth_buffer_pointer );
				glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
				glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE );
				glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE );
				glTexImage2D( GL_TEXTURE_2D , 0 , GL_DEPTH_COMPONENT32F ,
							  _desc._size._w , _desc._size._h , 0 , GL_DEPTH_COMPONENT , GL_FLOAT , 0 );
				glFramebufferTexture( GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , _depth_buffer_pointer , 0 );
			} else
			{
				glGenRenderbuffers( 1 , &_depth_buffer_pointer );
				glBindRenderbuffer( GL_RENDERBUFFER , _depth_buffer_pointer );
				glRenderbufferStorage( GL_RENDERBUFFER , GL_DEPTH_COMPONENT32F , _desc._size._w , _desc._size._h );
				glFramebufferRenderbuffer( GL_FRAMEBUFFER , GL_DEPTH_ATTACHMENT , GL_RENDERBUFFER , _depth_buffer_pointer );
			}
		}
	}
}
ViewTypes::RDrawBufferPTR RDrawPassGL::getBufferPtr( int i ) const
{
	if( N == 0 )
		return 0;
	return __texture_pointer_array[ i ];
}
uint RDrawPassGL::getBufferCount()
{
	return N;
}
void RDrawPassGL::clear( bool cd )
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
	} else
		glClear( GL_COLOR_BUFFER_BIT );
}
void RDrawPassGL::bind() const
{
	glBindFramebuffer( GL_FRAMEBUFFER , _framebuffer_id );
	glViewport( 0 , 0 , _desc._size._w , _desc._size._h );
}
ViewTypes::RDrawBufferPTR RDrawPassGL::getDepthBufferPtr() const
{
	return _depth_buffer_pointer;
}
void RDrawPassGL::release()
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
void RDrawPassGL::update( const RSize &size )
{
	if( _desc._size._w == size._w && _desc._size._h == size._h )
		return;
	_desc._size = size;
	release();
	init( _desc );
}
