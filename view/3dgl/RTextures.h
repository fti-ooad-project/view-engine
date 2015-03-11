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
	void getVal( uint *out , f2 const &tx ) const
	{
		/*f2 pos = tx & f2( _size._w , _size._h );
		f2 k = f2( pos.x() - floorf( pos.x() ) , pos.y() - floorf( pos.y() ) );
		ito( _bytes_per_pixel )
		{
			out[i] =
			( 1.0f - k.y() ) * (
			( 1.0f - k.x() ) * __data[int(pos.x())+int(pos.y())*_size._w]
			+ k.x() * __data[int(pos.x() + 1)+int(pos.y())*_size._w]
			)
			;
		}*/
	}
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
