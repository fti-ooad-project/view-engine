#ifndef HEIGHMAPDRAWLER_HPP
#define HEIGHMAPDRAWLER_HPP
#include "../../base/RBase.h"
#include "../GL.h"
#include "RTextures.h"
#include "RViewModelsGL.h"
#include "RTexturesGL.h"
class HeightMapDrawler : public RInitable
{
private:
	uint _vao;
	int _indx_count;
	RGraphicProgrammGL _prog;
	RGraphicProgrammGL _lightprog;
	RTextureHolderGL _textures;
	RTextureHolderGL _hmap;
	HeightMapDrawler() = default;
public:
	f3 _size;
	HeightMapDrawler( HeightMapDrawler const & ) = delete;
	void operator=( HeightMapDrawler const & ) = delete;
	void bindHeihgtTexture() const;
	void bindToDraw();
	void draw( bool );
	void init( uint , f3 const & );
	void release();
	static HeightMapDrawler *getSingleton();
};
#endif // HEIGHMAPDRAWLER_HPP
