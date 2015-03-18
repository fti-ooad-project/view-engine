#ifndef WATERSIMULATOR_HPP
#define WATERSIMULATOR_HPP
#include "../../base/RBase.h"
#include "RViewModelsGL.h"
#include "RPassGL.h"
#include "RDrawableGL.h"
#include "RTexturesGL.h"
class WaterSimulator : public RInitable
{
private:
	RGraphicProgrammGL _water_surf_prog , _water_bump_prog , _water_plane_prog;
	RDrawPassGL _water_bump_pass[ 2 ] , _water_surf_pass[ 2 ] , _final , _water_plane_pass;
	int cur = 0 , last = 1;
	RTextureHolderGL _wave_normal;
	RPolyQuadGL _screen_quad;
	f2 _size;
	float _height;
	f3 _last_cam_pos , _cam_pos;
	uint dest;
	WaterSimulator() = default;
	f4x4 water_viewproj;
public:
	WaterSimulator( WaterSimulator const & ) = delete;
	void operator=( WaterSimulator const & ) = delete;
	void init( int , f2 const & , float );
	void bindToRenderPlane( bool );
	void bindToRenderSurface();
	void switchSurfaceBuffer( f3 const & );
	void calc( float , float );
	uint getBumpTexture() const;
	uint getPlaneBuffer() const;
	uint getSurfBuffer() const;
	void release() override;
	static WaterSimulator *getSingleton();
};
#endif // WATERSIMULATOR_HPP
