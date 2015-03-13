#ifndef WATERSIMULATOR_HPP
#define WATERSIMULATOR_HPP
#include "../../base/RBase.h"
#include "RViewModelsGL.h"
#include "view/3dgl/RPassGL.h"
#include "view/3dgl/RDrawableGL.h"
class WaterSimulator : public RInitable
{
private:
	RGraphicProgrammGL _water_surf_prog , _water_bump_prog , _water_plane_prog;
	RDrawPassGL _water_bump_pass[2] , _water_surf_pass[2] , _final , _water_plane_pass;
	int cur = 0 , last = 1;
	RPolyQuadGL _screen_quad;
	WaterSimulator() = default;
public:
	WaterSimulator( WaterSimulator const & ) = delete;
	void operator=( WaterSimulator const & ) = delete;
	void init( uint depth_buf , f2 const &size , f3 const &pos )
	{
		if( isInited() ) return;
		setInited( true );
		_screen_quad.init();
		ito( 2 )
		{
			_water_surf_pass[i].init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 , false , false } );
			_water_bump_pass[i].init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 , false , false } );
			_water_bump_pass[i].bind();
			_water_bump_pass[i].clear();
		}
		_final.init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 , false , false } );
		_water_plane_pass.init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_FLOAT , 1 , depth_buf , false , false } );
		_water_plane_prog.init( "res/shaders/glsl/water_plane_fragment.glsl" , "res/shaders/glsl/water_plane_vertex.glsl" );
		_water_bump_prog.init( "res/shaders/glsl/water_bump_frag.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" );
		_water_surf_prog.init( "res/shaders/glsl/watersurf_frag.glsl" , "res/shaders/glsl/polymesh_tess_vertex.glsl" , "res/shaders/glsl/water_geometry.glsl" );
	}
	void bindToRenderPlane()
	{
		_water_plane_pass.bind();
		_water_plane_pass.clear( false );
		_water_plane_prog.bind();
	}
	void bindToRenderSurface()
	{
		last = cur;
		cur = ( cur + 1 ) % 2;
		_water_surf_pass[cur].bind();
		_water_surf_pass[cur].clear();
		_water_surf_prog.bind();
	}
	void calc()
	{
		_water_bump_prog.bind();
		glDepthFunc( GL_LEQUAL );
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D , _water_surf_pass[cur].getBufferPtr( 0 ) );
		glUniform1i( 0 , 0 );
		glActiveTexture( GL_TEXTURE0 + 1 );
		glBindTexture( GL_TEXTURE_2D , _water_surf_pass[last].getBufferPtr( 0 ) );
		glUniform1i( 1 , 1 );
		_water_bump_pass[cur].bind();
		glActiveTexture( GL_TEXTURE0 + 2 );
		glBindTexture( GL_TEXTURE_2D , _water_bump_pass[last].getBufferPtr( 0 ) );
		glUniform1i( 3 , 2 );
		glUniform1i( 2 , 0 );
		glUniform2f( 4 , 1.0f / 1024 , 1.0f / 1024 );
		_screen_quad.draw();
		_final.bind();
		_final.clear();
		glUniform1i( 2 , 1 );
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D , _water_bump_pass[cur].getBufferPtr( 0 ) );
		glUniform1i( 3 , 0 );
		_screen_quad.draw();
	}
	uint getBumpTexture() const
	{
		return _final.getBufferPtr( 0 );
	}
	uint getPlaneBuffer() const
	{
		return _water_plane_pass.getBufferPtr( 0 );
	}
	void release()
	{
		if( !isInited() ) return;
		setInited( false );
		_water_surf_prog.release();
		ito( 2 )
		{
			_water_surf_pass[i].release();
			_water_bump_pass[i].release();
		}
		_screen_quad.release();

	}
	static WaterSimulator *getSingleton()
	{
		static WaterSimulator sng[1];
		return sng;
	}
};
#endif // WATERSIMULATOR_HPP
