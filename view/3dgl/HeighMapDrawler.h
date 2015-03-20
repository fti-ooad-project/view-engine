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
	RTextureHolderGL _htex;
	RTextureHolderGL _diff;
	RTextureHolderGL _normtex;
	HeightMapDrawler() = default;
public:
	f2 _size;
	f2 _pos;
	HeightMapDrawler( HeightMapDrawler const & ) = delete;
	void operator=( HeightMapDrawler const & ) = delete;
	void bindToDraw();
	void draw(bool tess);
	void init(uint density, f2 const &size, f2 const &bpos);
	void release();
	static HeightMapDrawler *getSingleton();
};
#endif // HEIGHMAPDRAWLER_HPP
