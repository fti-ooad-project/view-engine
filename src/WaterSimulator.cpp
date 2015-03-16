#include "../view/3dgl/WaterSimulator.h"
#include "../view/export/RCamera.h"
#include "../GlslDefines.h"
void WaterSimulator::init( int depth_buf , f2 const &size , f3 const &pos )
{
	if( isInited() ) return;
	setInited( true );
	_pos = pos;
	_size = size;
	dest = 2048;
	water_viewproj = RCamera::orthographic( pos + f3( 0.0f , 0.0f , -10.0f ) , f3( 0.0f , 0.0f , 1.0f ) , f3( 0.0f , 1.0f , 0.0f ) , size.x() );
	_screen_quad.init();
	ito( 2 )
	{
		_water_surf_pass[ i ].init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 , false , false } );
		_water_bump_pass[ i ].init( { { dest , dest } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 , false , false } );
		_water_bump_pass[ i ].bind();
		_water_bump_pass[ i ].clear();
	}
	_final.init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 , false , false } );
	_water_plane_pass.init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_INT , 1 , depth_buf , false , false } );
	_water_plane_prog.init( "res/shaders/glsl/water_plane_fragment.glsl" , "res/shaders/glsl/water_plane_vertex.glsl" );
	_water_bump_prog.init( "res/shaders/glsl/water_bump_frag.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" );
	_water_surf_prog.init( "res/shaders/glsl/watersurf_frag.glsl" , "res/shaders/glsl/polymesh_tess_vertex.glsl" , "res/shaders/glsl/water_geometry.glsl" );
}
void WaterSimulator::bindToRenderPlane()
{
	_water_plane_pass.bind();
	_water_plane_pass.clear( false );
	_water_plane_prog.bind();
	glUniform4f( 4 , _pos.x() , _pos.y() , _pos.z() , _size.x() );
}
void WaterSimulator::bindToRenderSurface()
{
	last = cur;
	cur = ( cur + 1 ) % 2;
	_water_surf_pass[ cur ].bind();
	_water_surf_pass[ cur ].clear();
	_water_surf_prog.bind();
	glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE , water_viewproj.getPtr() );
	glUniform1f( 4 , _pos.z() );
}
void WaterSimulator::calc()
{
	_water_bump_prog.bind();
	glDepthFunc( GL_LEQUAL );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , _water_surf_pass[ cur ].getBufferPtr( 0 ) );
	glUniform1i( 0 , 0 );
	glActiveTexture( GL_TEXTURE0 + 1 );
	glBindTexture( GL_TEXTURE_2D , _water_surf_pass[ last ].getBufferPtr( 0 ) );
	glUniform1i( 1 , 1 );
	_water_bump_pass[ cur ].bind();
	glActiveTexture( GL_TEXTURE0 + 2 );
	glBindTexture( GL_TEXTURE_2D , _water_bump_pass[ last ].getBufferPtr( 0 ) );
	glUniform1i( 3 , 2 );
	glUniform1i( 2 , 0 );
	glUniform2f( 4 , 1.0f / dest , 1.0f / dest );
	_screen_quad.draw();
	_final.bind();
	_final.clear();
	glUniform1i( 2 , 1 );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , _water_bump_pass[ cur ].getBufferPtr( 0 ) );
	glUniform1i( 3 , 0 );
	_screen_quad.draw();
}
uint WaterSimulator::getBumpTexture() const
{
	return _final.getBufferPtr( 0 );
}
uint WaterSimulator::getPlaneBuffer() const
{
	return _water_plane_pass.getBufferPtr( 0 );
}
void WaterSimulator::release()
{
	if( !isInited() ) return;
	setInited( false );
	_water_surf_prog.release();
	ito( 2 )
	{
		_water_surf_pass[ i ].release();
		_water_bump_pass[ i ].release();
	}
	_screen_quad.release();

}
WaterSimulator *WaterSimulator::getSingleton()
{
	static WaterSimulator sng[ 1 ];
	return sng;
}