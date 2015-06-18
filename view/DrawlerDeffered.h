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
struct View
{
	int _flags;
	int _mesh_id;
	int _texture_id;
	int _anim_id;
};
struct InstanceInfo
{
	float time;
	float last_time;
	float dist_to_cam;
	float selection_id;
	int mixing;
	int cur_anim;
	int last_anim;
	int auto_height;
	f3 pos;
	f3 look;
	f3 left;
	f3 up;
};
class DrawlerDeffered : public Initable , public Timer
{
private:
#define LIGHT_CASTER_COUNT 1
	DrawlerDeffered() = default;
	Blurer _blurer;
	GraphicProgrammGL _storage_prog , _storage_tess_prog , _process_prog , _water_prog;
	DrawPassGL _light_dir_passes[LIGHT_CASTER_COUNT];
	//RDrawPassGL _light_cube_passes[LIGHT_CASTER_COUNT];
	DrawPassGL  _storage_pass , _process_pass , _water_pass;
	TextureHolderGL _env_tex , _lightk_tex;
	PolyQuadGL _screen_quad;
	void drawInstances( std::vector< InstanceInfo > const * );
	void drawInstancesToLight( std::vector< InstanceInfo > const * );
	void bindUniforms( View const & ) const;
	void updateRes();
	uint loadView( std::string , int , int pref_anim = -1 );
	u2 _resolution;
	f2 _mpos;
	f3 _wmpos;
	GraphicSettings _graph_settings;
public:
	std::vector< std::unique_ptr< PolyMeshGL > > _meshes;
	std::vector< TextureHolderGL > _textures;
	std::vector< BoneAnimInTexHolderGL > _animations;
	std::vector< View > _view;
	void init();
	f3 getMousePos( f2 const & );
	void release();
	uint draw( Scene3D const * , u2 const & );
	static DrawlerDeffered *getSingleton();
};
#endif // DRAWLERDEFFERED_HPP
