#ifndef HEIGHMAPDRAWLER_HPP
#define HEIGHMAPDRAWLER_HPP
#include "../../base/RBase.h"
class HeightMapDrawler : public RInitable
{
private:
	std::unique_ptr< RImage[] > &&_heightmap;
	f2 _size;
	f2 _pos;
public:
	HeightMapDrawler( std::unique_ptr< RImage[] > &&heightmap , f2 const &size , f2 const &pos ):
	_heightmap( std::move( heightmap ) )
	, _size( size )
	, _pos( pos )
	{
	}
	void init()
	{
	}
	void release()
	{
	}
};
#endif // HEIGHMAPDRAWLER_HPP
