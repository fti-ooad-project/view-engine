#ifndef RTEXTURESGL_H
#define RTEXTURESGL_H
#include "RTextures.h"
#include "../GL.h"
class RTextureHolderGL
{
private:
	std::unique_ptr< RSize[] > __tex_size;
	std::unique_ptr< RImage[] > _imgs;
public:
	uint _count;
	std::unique_ptr< uint[] > __texture_pointer_array;
	RTextureHolderGL( std::unique_ptr< RImage[] > &&imgs , int count ):
	_count( count )
	{
		if( !_count ) return;
		_imgs = std::move( imgs );
	}
	void init()
	{
		__texture_pointer_array = std::move( std::unique_ptr< uint[] >( new uint[_count] ) );
		__tex_size = std::move( std::unique_ptr< RSize[] >( new RSize[_count] ) );
		glGenTextures( _count , __texture_pointer_array.get() );
		for ( int i = 0; i < _count; i++ )
		{
			__tex_size[i] = _imgs[i]._size;
			glBindTexture( GL_TEXTURE_2D , __texture_pointer_array[i] );
			glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , _imgs[i]._size._w , _imgs[i]._size._h , 0 ,
					GL_RGBA , GL_UNSIGNED_BYTE , _imgs[i].__data.get() );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER ,
					GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER ,
					GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S ,
					GL_CLAMP_TO_EDGE );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T ,
					GL_CLAMP_TO_EDGE );
		}
		_imgs.reset();
	}
	~RTextureHolderGL()
	{
		release();
	}
	void release()
	{
		if( !__texture_pointer_array ) return;
		glDeleteTextures( _count , __texture_pointer_array.get() );
		__texture_pointer_array.reset();
		__tex_size.reset();
	}
};
#endif // RTEXTURESGL_H
