#include "../view/3dgl/WaterSimulator.h"
#include "../view/export/RCamera.h"
#include "../GlslDefines.h"
#include "../base/RFileloader.h"
void WaterSimulator::init( int depth_buf , f2 const &size , float height )
{
	if( isInited() ) return;
	setInited( true );
	_height = height;
	_size = size;
	dest = 2048;
	
	_screen_quad.init();
	ito( 2 )
	{
		_water_surf_pass[ i ].init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_INT , 1 , -1 , false , false } );
		_water_bump_pass[ i ].init( { { dest , dest } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 , false , false } );
		_water_bump_pass[ i ].bind();
		_water_bump_pass[ i ].clear();
	}
	_final.init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 , false , false } );
	_water_plane_pass.init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_INT , 1 , depth_buf , false , false } );
	_water_plane_prog.init( "res/shaders/glsl/water_plane_fragment.glsl" , "res/shaders/glsl/water_plane_vertex.glsl" );
	_water_bump_prog.init( "res/shaders/glsl/water_bump_frag.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" );
	_water_surf_prog.init( "res/shaders/glsl/watersurf_frag.glsl" , "res/shaders/glsl/polymesh_tess_vertex.glsl" , "res/shaders/glsl/water_geometry.glsl" );
	_wave_normal.init( std::move( RFileLoader::loadImage( "res/view/images/wave.png" ) ) , 1 );
}
void WaterSimulator::bindToRenderPlane( bool nt )
{
	_water_plane_pass.bind();
	_water_plane_prog.bind();
	if( nt )
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D , _final.getBufferPtr( 0 ) );
		glUniform1i( RGB_NORMAL_A_HEIGHT_TEXTURE , 0 );
		glUniform1i( PASSID , 0 );
		glUniform4f( 4 , _cam_pos.x() , _cam_pos.y() , _height , _size.x() );
	} else
	{
		glUniform1i( PASSID , 1 );
		//glUniform4f( 4 , 0.0f , 0.0f , _height , 500.0f );
		glUniform4f( 4 , _cam_pos.x() , _cam_pos.y() , _height , _size.x() );
	}
}
void WaterSimulator::bindToRenderSurface()
{
	_water_surf_pass[ cur ].bind();
	//_water_surf_prog.bind();
	glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE , water_viewproj.getPtr() );
	glUniform1f( WATER_Z , _height );
}
void WaterSimulator::switchSurfaceBuffer( f3 const & cam_pos )
{
	if( !_caminit )
	{
		_caminit = true;
		_last_cam_pos = cam_pos;
		_cam_pos = cam_pos;
		water_viewproj = RCamera::orthographic( f3( _cam_pos.x() , _cam_pos.y() , _height - 10.0f ) , f3( 0.0f , 0.0f , 1.0f ) , f3( 0.0f , 1.0f , 0.0f ) , _size.x() );
	}
	if( cam_pos.g_dist2( _cam_pos ) > 300.0f )
	{
		_last_cam_pos = _cam_pos;
		_cam_pos = cam_pos;
		water_viewproj = RCamera::orthographic( f3( _cam_pos.x() , _cam_pos.y() , _height - 10.0f ) , f3( 0.0f , 0.0f , 1.0f ) , f3( 0.0f , 1.0f , 0.0f ) , _size.x() );
	}
	last = cur;
	cur = ( cur + 1 ) % 2;
	_water_surf_pass[ cur ].clear();
}
void WaterSimulator::clearPlaneBuf()
{
	_water_plane_pass.clear( false );
}
void WaterSimulator::calc( float time , float dt )
{
	_water_bump_prog.bind();
	glUniform1f( 6 , time );
	glUniform1f( 8 , dt );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , _water_surf_pass[ cur ].getBufferPtr( 0 ) );
	glUniform1i( 0 , 0 );
	glActiveTexture( GL_TEXTURE0 + 1 );
	glBindTexture( GL_TEXTURE_2D , _water_surf_pass[ last ].getBufferPtr( 0 ) );
	glUniform1i( 1 , 1 );
	_water_bump_pass[ cur ].bind();
	_water_bump_pass[ cur ].clear();
	glActiveTexture( GL_TEXTURE0 + 2 );
	glBindTexture( GL_TEXTURE_2D , _water_bump_pass[ last ].getBufferPtr( 0 ) );
	glUniform1i( 3 , 2 );
	glUniform1i( 2 , 0 );
	glUniform2f( 4 , 1.0f / dest , 1.0f / dest );
	f2 dr = f2( _cam_pos.x() - _last_cam_pos.x() , _cam_pos.y() - _last_cam_pos.y() ) / _size.x() / 2.0f;
	//dr.print();
	glUniform2f( 7 , -dr.x() , dr.y() );
	_screen_quad.draw();
	_last_cam_pos = _cam_pos;
	_final.bind();
	_final.clear();
	glUniform1i( 2 , 1 );

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , _water_bump_pass[ cur ].getBufferPtr( 0 ) );
	glUniform1i( 3 , 0 );

	glActiveTexture( GL_TEXTURE0 + 1 );
	glBindTexture( GL_TEXTURE_2D , _wave_normal.getTexture() );
	glUniform1i( 5 , 1 );

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
uint WaterSimulator::getSurfBuffer() const
{
	return _water_surf_pass[ cur ].getBufferPtr( 0 );
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
	_wave_normal.release();
	_water_plane_prog.release();
	_water_bump_prog.release();
	_water_plane_pass.release();
}
WaterSimulator *WaterSimulator::getSingleton()
{
	static WaterSimulator sng[ 1 ];
	return sng;
}