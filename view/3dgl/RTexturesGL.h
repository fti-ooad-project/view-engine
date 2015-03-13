#ifndef RTEXTURESGL_H
#define RTEXTURESGL_H
#include "RTextures.h"
#include "../GL.h"
class RTextureHolderGL : public RInitable
{
private:
	std::unique_ptr< RSize[] > __tex_size;
	std::unique_ptr< RImage[] > _imgs;
	uint _count = 0;
	std::unique_ptr< uint[] > __texture_pointer_array;
public:
	uint getCount() const
	{
		return _count;
	}
	uint getTexture( uint i ) const
	{
		return __texture_pointer_array[i];
	}
	RTextureHolderGL( RTextureHolderGL &&a )
	{
		*this = std::move( a );
	}
	void operator=( RTextureHolderGL &&a )
	{
		release();
		_count = a._count;
		setInited( a.isInited() );
		if( a.isInited() )
		{
			__tex_size = std::move( a.__tex_size );
			__texture_pointer_array = std::move( a.__texture_pointer_array );
		}else
		{
			__tex_size = std::move( std::unique_ptr< RSize[] >( new RSize[_count] ) );
			_imgs = std::move( a._imgs );
			__texture_pointer_array = std::move( std::unique_ptr< uint[] >( new uint[_count] ) );
		}
		a.release();
	}
	RTextureHolderGL() = default;
	RTextureHolderGL( std::unique_ptr< RImage[] > &&imgs , int count )
	{
		_count = count;
		_imgs = std::move( _imgs );
		__texture_pointer_array = std::move( std::unique_ptr< uint[] >( new uint[_count] ) );
		__tex_size = std::move( std::unique_ptr< RSize[] >( new RSize[_count] ) );
	}
	void init( std::unique_ptr< RImage[] > &&imgs , int count )
	{
		release();
		_count = count;
		_imgs = std::move( _imgs );
		__texture_pointer_array = std::move( std::unique_ptr< uint[] >( new uint[_count] ) );
		__tex_size = std::move( std::unique_ptr< RSize[] >( new RSize[_count] ) );
		init();
	}
	void init()
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
		glGenTextures( _count , __texture_pointer_array.get() );
		for( uint i = 0; i < _count; i++ )
		{
			__tex_size[i] = _imgs[i]._size;
			glBindTexture( GL_TEXTURE_2D , __texture_pointer_array[i] );
			//glEnable( GL_TEXTURE_2D );
			const int mipmaplevels = std::min( 10 , pow2( std::min( __tex_size[i]._w , __tex_size[i]._h ) ) );
			uint i_f , s;
			LOG<<_imgs[i]._bytes_per_pixel;
			switch( _imgs[i]._bytes_per_pixel )
			{
			case 4:
			{
				i_f = GL_RGBA8;
				s = GL_RGBA;
			}
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
			glTexStorage2D( GL_TEXTURE_2D , mipmaplevels , i_f , _imgs[i]._size._w , _imgs[i]._size._h );
			glTexSubImage2D( GL_TEXTURE_2D , 0 , 0 , 0 , _imgs[i]._size._w , _imgs[i]._size._h ,
				s , GL_UNSIGNED_BYTE , _imgs[i].__data.get() );
			glGenerateMipmap( GL_TEXTURE_2D );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER ,
					GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER ,
					GL_LINEAR_MIPMAP_LINEAR );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S ,
					GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T ,
					GL_REPEAT );
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
