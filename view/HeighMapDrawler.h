#ifndef HEIGHMAPDRAWLER_HPP
#define HEIGHMAPDRAWLER_HPP
#include <base/Base.h>
#include <openGL/ViewModelsGL.h>
#include <openGL/TexturesGL.h>
class HeightMapDrawler : public Initable
{
private:
	uint _vao;
	int _indx_count;
	GraphicProgrammGL _prog;
	GraphicProgrammGL _lightprog;
	TextureHolderGL _textures;
	TextureHolderGL _hmap;
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
