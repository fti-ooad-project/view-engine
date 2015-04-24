#ifndef DRAWLERDEFFERED_HPP
#define DRAWLERDEFFERED_HPP
#include <base/Base.h>
#include <openGL\DrawableGL.h>
#include <openGL\PassGL.h>
#include "WaterSimulator.h"
#include "HeighMapDrawler.h"
#include <openGL\ViewModelsGL.h>
#include <view\Camera.h>
#include "export\ViewInterface.h"
#include "SelectionDrawler.h"
#include "ComplexPolyMeshGL.h"
class DrawlerDeffered : public Initable , public Timer
{
private:
#define LIGHT_CASTER_COUNT 1
	DrawlerDeffered() = default;
	GraphicProgrammGL _storage_prog , _storage_tess_prog , _process_prog , _water_prog;
	DrawPassGL _light_dir_passes[LIGHT_CASTER_COUNT];
	//RDrawPassGL _light_cube_passes[LIGHT_CASTER_COUNT];
	DrawPassGL  _storage_pass , _process_pass , _water_pass;
	TextureHolderGL _env_tex , _lightk_tex;
	PolyQuadGL _screen_quad;
	void drawInstances( std::vector< InstanceInfo > const * , bool tess = false );
	void drawInstancesToLight( std::vector< InstanceInfo > const * );
	void updateRes();
	u2 _resolution;
public:
	std::vector< std::unique_ptr< ComplexPolyMeshGL > > _view;
	void init();
	void release();
	uint draw( Scene3D const * , u2 const & );
	static DrawlerDeffered *getSingleton();
};
#endif // DRAWLERDEFFERED_HPP
