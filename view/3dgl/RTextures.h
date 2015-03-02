#ifndef __RTEXTURES_H__
#define __RTEXTURES_H__
#include "base/RBase.h"
struct RImage: public RInitable
{
	RSize _size;
	uint _bytes_per_pixel;
	std::unique_ptr< char[] > __data;
	RImage( const RImage & ) = delete;
	RImage& operator=( const RImage & ) = delete;
	RImage( std::unique_ptr< char[] > &&data , RSize size , uint bpp ):
	__data( std::move( data ) )
	, _size( size )
	, _bytes_per_pixel( bpp )
	{}
	RImage() = default;
	RImage( RImage &&img )
	{
		_size = img._size;
		__data = std::move( img.__data );
		_bytes_per_pixel = img._bytes_per_pixel;
	}
	void operator=( RImage &&img )
	{
		_size = img._size;
		__data = std::move( img.__data );
		_bytes_per_pixel = img._bytes_per_pixel;
	}
	void release()
	{
		__data.reset();
	}
	~RImage()
	{
	}
};
#endif /* __RTEXTURES_H__ */
