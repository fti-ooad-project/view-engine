#ifndef __RTEXTURES_H__
#define __RTEXTURES_H__
#include "../../base/RBase.h"
struct RImage: public RInitable
{
	RSize _size;
	uint _bytes_per_pixel = 0;
	std::unique_ptr< char[] > __data;
	RImage( const RImage & ) = delete;
	RImage& operator=( const RImage & ) = delete;
	RImage(std::unique_ptr< char[] > &&data, RSize size, uint bpp);
	void getVal(uint *out, f2 const &tx) const;
	RImage() = default;
	RImage(RImage &&img);
	void operator=(RImage &&img);
	void release();
	~RImage();
};
#endif /* __RTEXTURES_H__ */
