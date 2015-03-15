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
class DrawlerDeffered : public RInitable , public RTimer
{
private:
	#define LIGHT_CASTER_COUNT 1
	DrawlerDeffered() = default;
	RGraphicProgrammGL _prog[0x4];
	RDrawPassGL _light_dir_passes[LIGHT_CASTER_COUNT];
	//RDrawPassGL _light_cube_passes[LIGHT_CASTER_COUNT];
	RDrawPassGL  _pass[0x3];
	RTextureHolderGL _env_tex;
	RPolyQuadGL _screen_quad;
	inline void drawInstances( std::vector< InstanceInfo > const *info , bool tess = false );
	inline void drawInstancesToLight( std::vector< InstanceInfo > const *info );
public:
	std::vector< std::unique_ptr< RPolyMeshGL > > _view;
	void init();
	void release();
	uint draw( Scene3D const *scene , int w , int h );
	static DrawlerDeffered *getSingleton();
};
#endif // DRAWLERDEFFERED_HPP
