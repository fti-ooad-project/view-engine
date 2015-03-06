#ifndef RTEXTURESGL_H
#define RTEXTURESGL_H
#include "RTextures.h"
#include "../GL.h"
class RTextureHolderGL : public RInitable
{
private:
	std::unique_ptr< RSize[] > __tex_size;
	std::unique_ptr< RImage[] > _imgs;
public:
	uint _count;
	std::unique_ptr< uint[] > __texture_pointer_array;
	RTextureHolderGL( std::unique_ptr< RImage[] > &&imgs , int count ):
	_count( count )
	, _imgs( std::move( imgs ) )
	{
	}
	RTextureHolderGL( RTextureHolderGL &&a ):
	_count( a._count )
	, _imgs( std::move( a._imgs ) )
	, __tex_size( std::move( a.__tex_size ) )
	, __texture_pointer_array( std::move( a.__texture_pointer_array ) )
	{
		setInited( a.isInited() );
		a.setInited( false );
	}
	void operator=( RTextureHolderGL &&a )
	{
		_count = a._count;
		_imgs = std::move( a._imgs );
		__tex_size = std::move( a.__tex_size );
		__texture_pointer_array = std::move( a.__texture_pointer_array );
		setInited( a.isInited() );
		a.setInited( false );
	}
	RTextureHolderGL() = default;
	void init()
	{
		if( isInited() ) return;
		setInited( true );
		__texture_pointer_array = std::move( std::unique_ptr< uint[] >( new uint[_count] ) );
		__tex_size = std::move( std::unique_ptr< RSize[] >( new RSize[_count] ) );
		glGenTextures( _count , __texture_pointer_array.get() );
		for ( uint i = 0; i < _count; i++ )
		{
			__tex_size[i] = _imgs[i]._size;
			glBindTexture( GL_TEXTURE_2D , __texture_pointer_array[i] );
			switch( _imgs[i]._bytes_per_pixel )
			{
			case 4:
				glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA8 , _imgs[i]._size._w , _imgs[i]._size._h , 0 ,
					GL_RGBA , GL_UNSIGNED_BYTE , _imgs[i].__data.get() );
			break;
			case 3:
				glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGB , _imgs[i]._size._w , _imgs[i]._size._h , 0 ,
					GL_RGB , GL_UNSIGNED_BYTE , _imgs[i].__data.get() );
			break;
			case 1:
				glTexImage2D( GL_TEXTURE_2D , 0 , GL_LUMINANCE , _imgs[i]._size._w , _imgs[i]._size._h , 0 ,
					GL_LUMINANCE , GL_UNSIGNED_BYTE , _imgs[i].__data.get() );
			break;
			}
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER ,
					GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER ,
					GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S ,
					GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T ,
					GL_CLAMP_TO_EDGE );
#ifdef RLOG
		LOG << "_________________________\n" << "texture generated:id:" << __texture_pointer_array[i] << "\n";
#endif
		}
		_imgs.reset();
	}
	~RTextureHolderGL()
	{
		release();
	}
	void release()
	{
		if( !isInited() ) return;
		setInited( false );
		if( __texture_pointer_array )
		{
			glDeleteTextures( _count , __texture_pointer_array.get() );
			__texture_pointer_array.reset();
		}
		__tex_size.reset();
	}
};
#endif // RTEXTURESGL_H
