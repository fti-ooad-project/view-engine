#ifndef DRAWLERDEFFERED_HPP
#define DRAWLERDEFFERED_HPP
#include "../../base/RBase.h"
#include "RDrawableGL.h"
#include "RPassGL.h"
#include "WaterSimulator.h"
#include "HeighMapDrawler.h"
#include "RViewModelsGL.h"
#include "../export/RCamera.h"
#include "../export/ViewInterface.h"
#include "SelectionDrawler.h"
class DrawlerDeffered : public RInitable , public RTimer
{
private:
#define LIGHT_CASTER_COUNT 1
	DrawlerDeffered() = default;
	RGraphicProgrammGL _storage_prog , _storage_tess_prog , _process_prog , _water_prog;
	RDrawPassGL _light_dir_passes[LIGHT_CASTER_COUNT];
	//RDrawPassGL _light_cube_passes[LIGHT_CASTER_COUNT];
	RDrawPassGL  _storage_pass , _process_pass , _water_pass;
	RTextureHolderGL _env_tex , _lightk_tex;
	RPolyQuadGL _screen_quad;
	void drawInstances( std::vector< InstanceInfo > const * , bool tess = false );
	void drawInstancesToLight( std::vector< InstanceInfo > const * );
	void updateRes();
	u2 _resolution;
public:
	std::vector< std::unique_ptr< RPolyMeshGL > > _view;
	void init();
	void release();
	uint draw( Scene3D const * , u2 const & );
	static DrawlerDeffered *getSingleton();
};
#endif // DRAWLERDEFFERED_HPP
