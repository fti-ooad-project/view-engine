#include "../view/3dgl/RTexturesGL.h"
#include "../view/GL.h"
uint RTextureHolderGL::getCount() const
{
	return _count;
}
uint RTextureHolderGL::getTexture() const
{
	return _texture_array_pointer;// __texture_pointer_array[ i ];
}
RTextureHolderGL::RTextureHolderGL( RTextureHolderGL &&a )
{
	*this = std::move( a );
}
void RTextureHolderGL::operator=( RTextureHolderGL &&a )
{
	release();
	_count = a._count;
	setInited( a.isInited() );
	if( a.isInited() )
	{
		_texture_array_pointer = a._texture_array_pointer;
		a._texture_array_pointer = 0;
		//__texture_pointer_array = std::move( a.__texture_pointer_array );
	} else
	{
		//__tex_size = std::move( std::unique_ptr< RSize[] >( new RSize[ _count ] ) );
		_imgs = std::move( a._imgs );
		//__texture_pointer_array = std::move( std::unique_ptr< uint[] >( new uint[ _count ] ) );
	}
	a.release();
}
RTextureHolderGL::RTextureHolderGL( std::unique_ptr< RImage[] > &&imgs , int count )
{
	_count = count;
	_imgs = std::move( imgs );
	//__texture_pointer_array = std::move( std::unique_ptr< uint[] >( new uint[ _count ] ) );
}
void RTextureHolderGL::init( std::unique_ptr< RImage[] > &&imgs , int count )
{
	*this = std::move( RTextureHolderGL( std::move( imgs ) , 1 ) );
	init();
}
void RTextureHolderGL::init()
{
	if( !_imgs ) return;
	if( isInited() || !_count ) return;
	setInited( true );
	auto pow2 = []( int n )
	{
		int out = 1;
		ito( 32 )
		{
			if( n & ( 1 << i ) )
				out = i;
		}
		return out;
	};
	uint i_f , s;
	switch( _imgs[ 0 ]._bytes_per_pixel )
	{
		case 4:
			i_f = GL_RGBA8;
			s = GL_RGBA;
		break;
		case 3:
			i_f = GL_RGB8;
			s = GL_RGB;
		break;
		case 1:
			i_f = GL_LUMINANCE;
			s = GL_LUMINANCE;
		break;
	}
	const int mipmaplevels = std::min( 10 , pow2( std::min( _imgs[ 0 ]._size._w , _imgs[ 0 ]._size._h ) ) );
	glGenTextures( 1 , &_texture_array_pointer );
	if( _count == 1 )
	{
		glBindTexture( GL_TEXTURE_2D , _texture_array_pointer );
		glTexStorage2D( GL_TEXTURE_2D , mipmaplevels , i_f , _imgs[ 0 ]._size._w , _imgs[ 0 ]._size._h );
		glTexSubImage2D( GL_TEXTURE_2D , 0 , 0 , 0 , _imgs[ 0 ]._size._w , _imgs[ 0 ]._size._h , s , GL_UNSIGNED_BYTE , _imgs[ 0 ].__data.get() );
		glGenerateMipmap( GL_TEXTURE_2D );
		glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT );
	} else
	{
		glBindTexture( GL_TEXTURE_2D_ARRAY , _texture_array_pointer );
		glTexStorage3D( GL_TEXTURE_2D_ARRAY , mipmaplevels , i_f , _imgs[ 0 ]._size._w , _imgs[ 0 ]._size._h , _count );
		for( uint i = 0; i < _count; i++ )
		{
			glTexSubImage3D( GL_TEXTURE_2D_ARRAY , 0 , 0 , 0 , i , _imgs[ 0 ]._size._w , _imgs[ 0 ]._size._h , 1 , s , GL_UNSIGNED_BYTE , _imgs[ i ].__data.get() );
		}
		glGenerateMipmap( GL_TEXTURE_2D_ARRAY );
		glTexParameteri( GL_TEXTURE_2D_ARRAY , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D_ARRAY , GL_TEXTURE_MIN_FILTER , GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D_ARRAY , GL_TEXTURE_WRAP_S , GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D_ARRAY , GL_TEXTURE_WRAP_T , GL_REPEAT );
	}
	_imgs.reset();
}
void RTextureHolderGL::setRepeat( bool ref )
{
	if( ref )
	{
		if( _count == 1 )
		{
			glBindTexture( GL_TEXTURE_2D , _texture_array_pointer );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT );
		} else
		{
			glBindTexture( GL_TEXTURE_2D_ARRAY , _texture_array_pointer );
			glTexParameteri( GL_TEXTURE_2D_ARRAY , GL_TEXTURE_WRAP_S , GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D_ARRAY , GL_TEXTURE_WRAP_T , GL_REPEAT );
		}
	} else
	{
		if( _count == 1 )
		{
			glBindTexture( GL_TEXTURE_2D , _texture_array_pointer );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE );
		} else
		{
			glBindTexture( GL_TEXTURE_2D_ARRAY , _texture_array_pointer );
			glTexParameteri( GL_TEXTURE_2D_ARRAY , GL_TEXTURE_WRAP_S , GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D_ARRAY , GL_TEXTURE_WRAP_T , GL_CLAMP_TO_EDGE );
		}
	}
}
RTextureHolderGL::~RTextureHolderGL()
{
	release();
}
void RTextureHolderGL::release()
{
	if( !isInited() ) return;
	setInited( false );
	/*if( __texture_pointer_array )
	{
		glDeleteTextures( _count , __texture_pointer_array.get() );
		__texture_pointer_array.reset();
	}*/glDeleteTextures( 1 , &_texture_array_pointer );
	//__tex_size.reset();
}