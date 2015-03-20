#ifndef RTEXTURESGL_H
#define RTEXTURESGL_H
#include "RTextures.h"
#include <algorithm>
class RTextureHolderGL : public RInitable
{
private:
	std::unique_ptr< RImage[] > _imgs;
	uint _count = 0;
	//std::unique_ptr< uint[] > __texture_pointer_array;
	uint _texture_array_pointer;
public:
	uint getCount() const;
	uint getTexture() const;
	RTextureHolderGL( RTextureHolderGL && );
	void operator=( RTextureHolderGL && );
	RTextureHolderGL() = default;
	RTextureHolderGL( std::unique_ptr< RImage[] > && , int );
	void init( std::unique_ptr< RImage[] > && , int );
	void init();
	~RTextureHolderGL();
	void release();
};
#endif // RTEXTURESGL_H
