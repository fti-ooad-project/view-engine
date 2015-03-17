#include <memory>
#include "../view/3dgl/DrawlerDeffered.h"
#include "../GlslDefines.h"
void DrawlerDeffered::drawInstances( std::vector<InstanceInfo> const *info , bool tess )
{
	if( false )
	{
		glPolygonMode( GL_FRONT_AND_BACK , GL_LINE );
		glLineWidth( 0.01f );
	}
	ito( _view.size() )
	{
		/*if( i == 1 )
		 {
		 glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		 glLineWidth( 0.01f );
		 }*/
		if( tess )
		{
			if( info[ 2 * i ].size() > 0 )
			{
				_storage_tess_prog.bind();
				_view[ i ]->drawInstancedPatches( info[ i * 2 ] );
			}
			if( info[ 2 * i + 1 ].size() > 0 )
			{
				_storage_prog.bind();
				_view[ i ]->drawInstanced( info[ i * 2 + 1 ] );
			}
		} else
		{
			_storage_prog.bind();
			if( info[ 2 * i ].size() > 0 )
			{
				_view[ i ]->drawInstanced( info[ i * 2 ] );
			}
			if( info[ 2 * i + 1 ].size() > 0 )
			{
				_view[ i ]->drawInstanced( info[ i * 2 + 1 ] );
			}
		}
	}
	//_heigmap_drawl.bindToDraw();
	//_heigmap_drawl.draw( true );
	glPolygonMode( GL_FRONT_AND_BACK , GL_FILL );
}
void DrawlerDeffered::drawInstancesToLight( std::vector<InstanceInfo> const *info )
{
	ito( _view.size() )
	{
		if( info[ 2 * i ].size() > 0 )
			_view[ i ]->drawInstanced( info[ i * 2 ] );
		if( info[ 2 * i + 1 ].size() > 0 )
			_view[ i ]->drawInstanced( info[ i * 2 + 1 ] );
	}
}
void DrawlerDeffered::init()
{
	if( isInited() )
		return;
	setInited( true );
	_screen_quad.init();
	_storage_tess_prog.init( "res/shaders/glsl/polymesh_frag.glsl" ,
					 "res/shaders/glsl/polymesh_tess_vertex.glsl" ,
					 "res/shaders/glsl/polymesh_tess_geom.glsl" ,
					 "res/shaders/glsl/polymesh_tess_tc.glsl" ,
					 "res/shaders/glsl/polymesh_tess_te.glsl" );
	_storage_prog.init( //"res/shaders/glsl/watersurf_frag.glsl" , "res/shaders/glsl/polymesh_tess_vertex.glsl" , "res/shaders/glsl/water_geometry.glsl"
					 "res/shaders/glsl/polymesh_frag.glsl" ,
					 "res/shaders/glsl/polymesh_vertex.glsl"
					 );
	_process_prog.init( "res/shaders/glsl/pass1_frag.glsl" ,
					 "res/shaders/glsl/screen_quad_vertex.glsl" , "" );
	_view.push_back(
		std::move(
		std::unique_ptr < RPolyMeshGL
		>( new RComplexPolyMeshGL(
		RFileLoader::loadPolyMeshBin(
		RFileLoader::getStream(
		"res/view/polymodels/monkey.bin" ) ,
		RPolymesh::RPolyMeshType::RBONED_PMESH ) ) ) ) );
	_view.push_back(
		std::move( std::unique_ptr < RPolyMeshGL >( new RPolyBoxGL ) ) );
	_view.push_back(
		std::move(
		std::unique_ptr < RPolyMeshGL
		>( new RComplexPolyMeshGL(
		RFileLoader::loadPolyMeshBin(
		RFileLoader::getStream(
		"res/view/polymodels/tower.bin" ) ,
		RPolymesh::RPolyMeshType::RSTATIC_PMESH ) ) ) ) );
	for( std::unique_ptr<RPolyMeshGL> &i : _view )
	{
		i->init();
		i->genInstancedBuffer();
	}
	ito( LIGHT_CASTER_COUNT )
	{
		_light_dir_passes[ i ].init( { { 1024 , 1024 },
				RBufferStoreType::RBUFFER_FLOAT, 0, -1, true, false } );
		//_light_cube_passes[i].init( { { 512 , 512 } , RBufferStoreType::RBUFFER_FLOAT , 0 , -1 , true , true } );
	}
	_storage_pass.init( { { 1024, 1024 }, RBufferStoreType::RBUFFER_INT, 1, -1,
			false, false } );
	_process_pass.init( { { 1024, 1024 }, RBufferStoreType::RBUFFER_FLOAT, 1, -1,
			false, false } );
	_env_tex.init(
		std::move(
		RFileLoader::loadImage(
		"res/view/images/winter.jpg" ) ) ,
		1 );
	HeightMapDrawler::getSingleton()->init( 100 , f2( 500.0f , 500.0f ) ,
											f2( 0.0f , 0.0f ) );
	WaterSimulator::getSingleton()->init( _storage_pass.getDepthBufferPtr() ,
										  f2( 250.0f , 250.0f ) , f3( 0.0f , 0.0f , -20.0f ) );
	SelectionDrawler::getSingleton()->init();
}
void DrawlerDeffered::release()
{
	if( !isInited() )
		return;
	setInited( false );
	_process_pass.release();
	_storage_pass.release();
	_storage_tess_prog.release();
	_process_prog.release();
	_storage_prog.release();
	_view.clear();
	_screen_quad.release();
	_env_tex.release();
	HeightMapDrawler::getSingleton()->release();
	WaterSimulator::getSingleton()->release();
	SelectionDrawler::getSingleton()->release();
}
uint DrawlerDeffered::draw( Scene3D const *scene , int w , int h )
{
	updateTime();
	if( !scene )
		return 0;
	std::unique_ptr< std::vector<InstanceInfo>[] > data( new std::vector<InstanceInfo>[ _view.size() * 2 ] );
	float time = _cur_time - floorf( _cur_time );
	scene->getCamera()->setAspect(
		float( w ) / std::max( w , h ) * 3.14f * 0.5f ,
		float( h ) / std::max( w , h ) * 3.14f * 0.5f );
	for( RDrawableState const &ins : scene->getStateVector() )
	{
		f4x4 const &m = ins._view[ 0 ].model;
		f3 pos = f3( m( 3 , 0 ) , m( 3 , 1 ) , m( 3 , 2 ) );

		float cam_dist = pos.g_dist( scene->getCamera()->_v3pos )
			/ _view[ ins._view[ 0 ].view_id ]->_size.z() * 2.0f;
		if( cam_dist < 3.0f )
			data[ ins._view[ 0 ].view_id * 2 ].push_back( { 0.2f * time, 0.0f,
					cam_dist, 1.0f, 0, 0, 0, m } );
		else
		{
			if( !scene->getCamera()->fristrum( pos ,
				_view[ ins._view[ 0 ].view_id ]->_size.z() ) )
				continue;

			if( cam_dist < 50.0f )
				data[ ins._view[ 0 ].view_id * 2 + 1 ].push_back(
			{ 0.2f * time, 0.0f, cam_dist, 1.0f , 0, 0, 0, m } );
			else
			{
				f4x4 mn = m;
				mn.scale( _view[ ins._view[ 0 ].view_id ]->_size );
				data[ 3 ].push_back( { 0.2f * time, 0.0f, cam_dist, 1.0f , 0, 0, 0, mn } );
			}
		}
	}
	if( time < 0.01f )
		LOG << 1.0f / _dt << "\n";
	_storage_pass.clear();
	_storage_tess_prog.bind();
	glUniform1f( TIME , time );
	glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE ,
						scene->getCamera()->getViewProj().getPtr() );
	glUniform1i( LIGHTPASS , 0 );
	glUniform1i( INSTANSING , 1 );
	glUniform3fv( CAM_POS , 1 , scene->getCamera()->_v3pos.getArray() );
	_storage_prog.bind();
	glUniform1f( TIME , time );
	glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE ,
						scene->getCamera()->getViewProj().getPtr() );
	glUniform1i( INSTANSING , 1 );
	glUniform1i( LIGHTPASS , 0 );
	glUniform3fv( CAM_POS , 1 , scene->getCamera()->_v3pos.getArray() );
	//
	drawInstances( data.get() , true );

	HeightMapDrawler::getSingleton()->bindToDraw();
	glUniform1i( LIGHTPASS , 0 );
	glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE ,
						scene->getCamera()->getViewProj().getPtr() );
	glUniform3fv( CAM_POS , 1 , scene->getCamera()->_v3pos.getArray() );
	HeightMapDrawler::getSingleton()->draw( true );
	
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	//glLineWidth( 0.01f );

	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	/*f4x4 model( 100.0f );
	 _prog[2].bind();
	 glUniform3fv( 11 , 1 , scene->getCamera()->_v3pos.getArray() );
	 glUniformMatrix4fv( 6 , 1 , GL_FALSE , model.getPtr() );
	 glActiveTexture( GL_TEXTURE0 );
	 glBindTexture( GL_TEXTURE_2D , WaterSimulator::getSingleton()->getBumpTexture() );
	 glUniform1i( 1 , 0 );
	 glUniform1i( 15 , 0 );
	 glUniform1f( 16 , 0 );
	 glUniform1i( 0 , 0 );
	 _screen_quad.draw();*/

	 ///lights
	int caster_dir_light_count = 0 , caster_omni_light_count = 0;
	RLightState const *caster_dir_light_ptr[ LIGHT_CASTER_COUNT ] ,
		*caster_omni_light_ptr[ LIGHT_CASTER_COUNT ];
	f4x4 dir_lights_viewproj[ LIGHT_CASTER_COUNT ];

	for( RLightState const &l : scene->getLightVector() )
	{
		if( l._type == RLightSourceType::RLIGHT_DIRECT )
		{
			if( caster_dir_light_count > 2 || !l._cast_shadow )
				continue;
			caster_dir_light_ptr[ caster_dir_light_count ] = &l;
			_light_dir_passes[ caster_dir_light_count ].bind();
			_light_dir_passes[ caster_dir_light_count ].clear();

			dir_lights_viewproj[ caster_dir_light_count ] = RCamera::orthographic(
				l._pos , l._dir , f3( 0.0f , 0.0f , 1.0f ) , 300.0f );

			_storage_prog.bind();
			glUniform1i( LIGHTPASS , 1 );
			glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE ,
								dir_lights_viewproj[ caster_dir_light_count ].getPtr() );
			drawInstancesToLight( data.get() );

			HeightMapDrawler::getSingleton()->bindToDraw();
			glUniform1i( LIGHTPASS , 1 );
			glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE ,
								dir_lights_viewproj[ caster_dir_light_count ].getPtr() );
			HeightMapDrawler::getSingleton()->draw( true );

			caster_dir_light_count++;
		} else
		{
			if( caster_omni_light_count > 2 )
				continue;
			caster_omni_light_count++;
		}
	}

	///water
	 WaterSimulator::getSingleton()->bindToRenderSurface();
	 glUniform1f( TIME , time );
	 glUniform1i( INSTANSING , 1 );
	 drawInstancesToLight( data.get() );
	 //HeightMapDrawler::getSingleton()->draw( false );
	 WaterSimulator::getSingleton()->calc();
	 //_pass[0].bind();
	 WaterSimulator::getSingleton()->bindToRenderPlane();
	 glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE , scene->getCamera()->getViewProj().getPtr() );
	 glActiveTexture( GL_TEXTURE0 );
	 glBindTexture( GL_TEXTURE_2D , WaterSimulator::getSingleton()->getBumpTexture() );
	 glUniform1i( RGB_NORMAL_A_HEIGHT_TEXTURE , 0 );
	 _screen_quad.draw();
	 ///

	 ///
	 uint select_buff = SelectionDrawler::getSingleton()->process( _storage_pass.getBufferPtr( 0 ) );
	 ///

	_process_pass.clear();
	_process_prog.bind();

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , _storage_pass.getBufferPtr( 0 ) );
	glUniform1i( BUFFER0 , 0 );
	glActiveTexture( GL_TEXTURE0 + 1 );
	glBindTexture( GL_TEXTURE_2D , _env_tex.getTexture( 0 ) );
	glUniform1i( ENV , 1 );
	glActiveTexture( GL_TEXTURE0 + 2 );
	glBindTexture( GL_TEXTURE_2D , select_buff );
	glUniform1i( 2 , 2 );
	///
	glActiveTexture( GL_TEXTURE0 + 4 );
	glBindTexture( GL_TEXTURE_2D , WaterSimulator::getSingleton()->getPlaneBuffer() );
	glUniform1i( 11 , 4 );
	///
	glUniform1i( DLIGHT_COUNT , caster_dir_light_count );
	glUniform3fv( CAM , 1 , scene->getCamera()->_v3pos.getArray() );
	glUniform3fv( CAM + 1 , 1 , scene->getCamera()->_v3local_z.getArray() );
	f3 cx = scene->getCamera()->_v3local_x
		* tanf( scene->getCamera()->_fovx / 2.0f );
	f3 cy = scene->getCamera()->_v3local_y
		* tanf( scene->getCamera()->_fovy / 2.0f );
	glUniform3fv( CAM + 2 , 1 , cx.getArray() );
	glUniform3fv( CAM + 3 , 1 , cy.getArray() );
	ito( caster_dir_light_count )
	{
		int lid = DLIGHT + i * 4;
		glUniform3fv( lid , 1 , caster_dir_light_ptr[ i ]->_dir.getArray() );
		glUniform4fv( lid + 1 , 1 , caster_dir_light_ptr[ i ]->_colori.getArray() );
		glUniformMatrix4fv( lid + 2 , 1 , GL_FALSE ,
							dir_lights_viewproj[ i ].getPtr() );
		glActiveTexture( GL_TEXTURE0 + 5 + i );
		glBindTexture( GL_TEXTURE_2D , _light_dir_passes[ i ].getDepthBufferPtr() );
		glUniform1i( lid + 3 , 5 + i );
	}
	_screen_quad.draw();
	return _process_pass.getBufferPtr( 0 );
}
DrawlerDeffered *DrawlerDeffered::getSingleton()
{
	static DrawlerDeffered sng[ 1 ];
	return sng;
}