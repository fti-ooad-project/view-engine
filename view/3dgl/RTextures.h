#ifndef __RTEXTURES_H__
#define __RTEXTURES_H__
#include "base/RBase.h"
#include "view/3dgl/RViewModels.h"
struct RImage: public RInitable
{
	RSize _size;
	std::unique_ptr< char > __data;
	RImage( const RImage & ) = delete;
	RImage& operator=( const RImage & ) = delete;
	RImage( std::unique_ptr< char > &&data , RSize size ): __data( std::move( data ) ) , _size( size ){}
	RImage() = default;
	RImage( RImage &&img )
	{
		_size = img._size;
		__data = std::move( img.__data );
	}
	void operator=( RImage &&img )
	{
		_size = img._size;
		__data = std::move( img.__data );
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
