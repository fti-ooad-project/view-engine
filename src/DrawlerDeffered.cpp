#include <memory>
#include "../view/3dgl/DrawlerDeffered.h"
#include "../GlslDefines.h"
#define LIGHTCASTERS
#define RENDERWATER
void DrawlerDeffered::drawInstances( std::vector<InstanceInfo> const *info , bool tess )
{
	ito( _view.size() )
	{
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
void DrawlerDeffered::updateRes()
{
	_storage_pass.release();
	_storage_pass.init( RPassDesc{ { _resolution.x() , _resolution.y() } , RBufferStoreType::RBUFFER_INT , 1 , -1 , false , false , 4 } );
	_process_pass.release();
	_process_pass.init( RPassDesc{ { _resolution.x() , _resolution.y() } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 , false , false , 3 } );
	_water_pass.release();
	_water_pass.init( RPassDesc{ { _resolution.x() , _resolution.y() } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 , false , false , 3 } );
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
						"res/shaders/glsl/polymesh_tess_vertex.glsl" ,
						"res/shaders/glsl/polymesh_geometry.glsl"
						);
	_process_prog.init( "res/shaders/glsl/pass1_frag.glsl" ,
						"res/shaders/glsl/screen_quad_vertex.glsl" , "" );
	_water_prog.init( "res/shaders/glsl/pass2_frag.glsl" ,
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
	_view.push_back(
		std::move(
		std::unique_ptr < RPolyMeshGL
		>( new RComplexPolyMeshGL(
		RFileLoader::loadPolyMeshBin(
		RFileLoader::getStream(
		"res/view/polymodels/sword.bin" ) ,
		RPolymesh::RPolyMeshType::RSTATIC_PMESH ) ) ) ) );
	for( std::unique_ptr<RPolyMeshGL> &i : _view )
	{
		i->init();
		i->genInstancedBuffer();
	}
	ito( LIGHT_CASTER_COUNT )
	{
		_light_dir_passes[ i ].init( { { 1024 , 1024 } ,
									 RBufferStoreType::RBUFFER_FLOAT , 0 , -1 , true , false } );
		//_light_cube_passes[i].init( { { 512 , 512 } , RBufferStoreType::RBUFFER_FLOAT , 0 , -1 , true , true } );
	}
	_storage_pass.init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_INT , 1 , -1 ,
						false , false , 4 } );
	_process_pass.init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 ,
						false , false , 3 } );
	_water_pass.init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 ,
					  false , false , 3 } );
	_env_tex.init( std::move( RFileLoader::loadImage( "res/view/images/sky2.jpg" ) ) , 1 );
	_lightk_tex.init( std::move( RFileLoader::loadImage( "res/view/images/lightk.png" ) ) , 1 );
	_lightk_tex.setRepeat( false );
	HeightMapDrawler::getSingleton()->init( 100 , f3( 500.0f , 500.0f , 50.0f ) );
#ifdef RENDERWATER
	WaterSimulator::getSingleton()->init( _storage_pass.getDepthBufferPtr() ,
										  f2( 100.0f , 100.0f ) , 0.0f );
#endif
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
#ifdef RENDERWATER
	WaterSimulator::getSingleton()->release();
#endif
	SelectionDrawler::getSingleton()->release();
}
uint DrawlerDeffered::draw( Scene3D const *scene , u2 const &res )
{
	updateTime();
	if( !scene )
		return 0;
	//if( _resolution != res )
	//{
	_resolution = res;
	scene->getCamera()->setAspect(
		float( _resolution.x() ) / std::max( _resolution.x() , _resolution.y() ) * 3.14f * 0.5f ,
		float( _resolution.y() ) / std::max( _resolution.x() , _resolution.y() ) * 3.14f * 0.5f );
	//}

	std::unique_ptr< std::vector<InstanceInfo>[] > data( new std::vector<InstanceInfo>[ _view.size() * 2 ] );
	float time = _cur_time - floorf( _cur_time );
	for( UnitInstanceState const &ins : scene->getStateVector() )
	{
		float cam_dist = f2( ins._pos.x() , ins._pos.y() ).g_dist( f2( scene->getCamera()->_v3pos.x() , scene->getCamera()->_v3pos.y() ) ) / _view[ ins._view[ 0 ] ]->_size.z();
		ins._animstat.update( _dt );
		for( auto const i : ins._view )
		{
			InstanceInfo instance_info{
				ins._animstat._moment._moment , ins._animstat._moment._last_moment
				, cam_dist , 1.0f , int( ins._animstat._moment._mixing )
				, ins._animstat._moment._cur_set , ins._animstat._moment._last_set , int( ins._auto_height ) , ins._pos , ins._look , vecx( ins._look , ins._up ) , ins._up };
			//if( cam_dist < 3.0f )
			data[ i * 2 ].push_back( instance_info );
			/*else
			{
				if( !scene->getCamera()->fristrum2d( ins._pos ) )
					continue;

				if( cam_dist < 15.0f )
					data[ i * 2 + 1 ].push_back( instance_info );
				else
				{
					data[ 3 ].push_back( instance_info );
				}
			}*/
		}
	}
	if( time < 0.01f )
		LOG << 1.0f / _dt << "\n";
	_storage_pass.clear();
	_storage_tess_prog.bind();
	HeightMapDrawler::getSingleton()->bindHeihgtTexture();
	glUniform1f( TIME , time );
	glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE , scene->getCamera()->getViewProj().getPtr() );
	glUniform1i( PASSID , PASS_NORMAL );
	glUniform1i( INSTANSING , 1 );
	glUniform3fv( CAM_POS , 1 , scene->getCamera()->_v3pos.getArray() );
	_storage_prog.bind();
	HeightMapDrawler::getSingleton()->bindHeihgtTexture();
	glUniform1f( TIME , time );
	glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE ,
						scene->getCamera()->getViewProj().getPtr() );
	glUniform1i( INSTANSING , 1 );
	glUniform1i( PASSID , PASS_NORMAL );
	glUniform3fv( CAM_POS , 1 , scene->getCamera()->_v3pos.getArray() );
	//
	//glPolygonMode( GL_FRONT_AND_BACK , GL_LINE );
	drawInstances( data.get() , true );
	//glPolygonMode( GL_FRONT_AND_BACK , GL_FILL );
	//glDisable( GL_CULL_FACE );
	HeightMapDrawler::getSingleton()->bindToDraw();
	glUniform1i( PASSID , PASS_NORMAL );
	glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE ,
						scene->getCamera()->getViewProj().getPtr() );
	glUniform3fv( CAM_POS , 1 , scene->getCamera()->_v3pos.getArray() );
	HeightMapDrawler::getSingleton()->draw( true );

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
#ifdef LIGHTCASTERS
	int caster_dir_light_count = 0 , caster_omni_light_count = 0;
	LightState const *caster_dir_light_ptr[ LIGHT_CASTER_COUNT ] ,
		*caster_omni_light_ptr[ LIGHT_CASTER_COUNT ];
	f4x4 dir_lights_viewproj[ LIGHT_CASTER_COUNT ];

	for( LightState const &l : scene->getLightVector() )
	{
		if( l._type == RLightSourceType::RLIGHT_DIRECT )
		{
			if( caster_dir_light_count > 2 || !l._cast_shadow )
				continue;
			caster_dir_light_ptr[ caster_dir_light_count ] = &l;
			//_light_dir_passes[ caster_dir_light_count ].bind();
			_light_dir_passes[ caster_dir_light_count ].clear();

			dir_lights_viewproj[ caster_dir_light_count ] = RCamera::orthographic(
				l._pos , l._dir , f3( 0.0f , 0.0f , 1.0f ) , 300.0f );

			_storage_prog.bind();
			glUniform1i( PASSID , PASS_LIGHT );
			glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE ,
								dir_lights_viewproj[ caster_dir_light_count ].getPtr() );
			drawInstancesToLight( data.get() );

			HeightMapDrawler::getSingleton()->bindToDraw();
			glUniform1i( PASSID , PASS_LIGHT );
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
#endif
	///water
#ifdef RENDERWATER
	glDisable( GL_CULL_FACE );
	//_storage_tess_prog.bind();
	//WaterSimulator::getSingleton()->bindToRenderSurface();
	//glUniform1i( PASSID , PASS_WATER );
	WaterSimulator::getSingleton()->switchSurfaceBuffer( scene->getCamera()->_v3pos );
	_storage_prog.bind();
	WaterSimulator::getSingleton()->bindToRenderSurface();
	glUniform1i( PASSID , PASS_WATER );

	glPolygonMode( GL_FRONT_AND_BACK , GL_LINE );
	glLineWidth( 0.02f );
	drawInstances( data.get() , false );
	glPolygonMode( GL_FRONT_AND_BACK , GL_FILL );

	HeightMapDrawler::getSingleton()->bindToDraw();
	WaterSimulator::getSingleton()->bindToRenderSurface();
	glUniform1i( PASSID , PASS_WATER );
	HeightMapDrawler::getSingleton()->draw( true );

	//HeightMapDrawler::getSingleton()->bindToDraw();
	WaterSimulator::getSingleton()->calcReflectionCamera( *scene->getCamera() );
	WaterSimulator::getSingleton()->bindToRenderReflection();
	glUniform1i( PASSID , PASS_REFLECT );
	HeightMapDrawler::getSingleton()->draw( true );
	_storage_prog.bind();
	glUniform1i( PASSID , PASS_REFLECT );
	WaterSimulator::getSingleton()->bindToRenderReflection();
	drawInstances( data.get() , false );

	//glEnable( GL_CULL_FACE );
	//HeightMapDrawler::getSingleton()->draw( false );
	WaterSimulator::getSingleton()->calc( _cur_time , _dt );
	//_pass[0].bind();
	WaterSimulator::getSingleton()->clearPlaneBuf();
	WaterSimulator::getSingleton()->bindToRenderPlane( false );
	glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE , scene->getCamera()->getViewProj().getPtr() );
	_screen_quad.draw();
	//glDepthFunc( GL_LESS );
	WaterSimulator::getSingleton()->bindToRenderPlane( true );
	_screen_quad.draw();
	glEnable( GL_CULL_FACE );
	///
#endif
	///
	uint select_buff = SelectionDrawler::getSingleton()->process( _storage_pass.getBufferPtr( 0 ) );
	///

	_process_pass.clear();
	_process_prog.bind();

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , _storage_pass.getBufferPtr( 0 ) );
	glUniform1i( BUFFER0 , 0 );
	glActiveTexture( GL_TEXTURE0 + 1 );
	glBindTexture( GL_TEXTURE_2D , _env_tex.getTexture() );
	glUniform1i( ENV , 1 );
	glActiveTexture( GL_TEXTURE0 + 2 );
	glBindTexture( GL_TEXTURE_2D , select_buff );
	glUniform1i( 2 , 2 );
	glActiveTexture( GL_TEXTURE0 + 3 );
	glBindTexture( GL_TEXTURE_2D , _lightk_tex.getTexture() );
	glUniform1i( 8 , 3 );
	glActiveTexture( GL_TEXTURE0 + 6 );
	glBindTexture( GL_TEXTURE_2D , WaterSimulator::getSingleton()->getReflectionPass() );
	glUniform1i( 12 , 6 );
	/*glActiveTexture( GL_TEXTURE0 + 3 );
	glBindTexture( GL_TEXTURE_2D_ARRAY , dynamic_cast< RComplexPolyMeshGL* >( _view[0].get() )->__anim_intex.getBufferPtr() );
	glUniform1i( 3 , 3 );*/
#ifdef RENDERWATER
	glActiveTexture( GL_TEXTURE0 + 4 );
	glBindTexture( GL_TEXTURE_2D , WaterSimulator::getSingleton()->getPlaneBuffer() );
	glUniform1i( 11 , 4 );
#endif

	glUniform3fv( CAM , 1 , scene->getCamera()->_v3pos.getArray() );
	glUniform3fv( CAM + 1 , 1 , scene->getCamera()->_v3local_z.getArray() );
	f3 cx = scene->getCamera()->_v3local_x * tanf( scene->getCamera()->_fovx / 2.0f );
	f3 cy = scene->getCamera()->_v3local_y * tanf( scene->getCamera()->_fovy / 2.0f );
	glUniform3fv( CAM + 2 , 1 , cx.getArray() );
	glUniform3fv( CAM + 3 , 1 , cy.getArray() );
	glUniformMatrix4fv( 3 , 1 , GL_FALSE , scene->getCamera()->getViewProj().getPtr() );
#ifdef LIGHTCASTERS
	glUniform1i( DLIGHT_COUNT , caster_dir_light_count );
	ito( caster_dir_light_count )
	{
		int lid = DLIGHT + i * 4;
		glUniform3fv( lid , 1 , caster_dir_light_ptr[ i ]->_dir.getArray() );
		glUniform4fv( lid + 1 , 1 , caster_dir_light_ptr[ i ]->_colori.getArray() );
		glUniformMatrix4fv( lid + 2 , 1 , GL_FALSE , dir_lights_viewproj[ i ].getPtr() );
		glActiveTexture( GL_TEXTURE0 + 5 + i );
		glBindTexture( GL_TEXTURE_2D , _light_dir_passes[ i ].getDepthBufferPtr() );
		glUniform1i( lid + 3 , 5 + i );
	}
#endif
	_screen_quad.draw();

	/*_water_pass.clear();
	_water_pass.bind();
	_water_prog.bind();
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , _process_pass.getBufferPtr( 0 ) );
	glUniform1i( 0 , 0 );
	glActiveTexture( GL_TEXTURE0 + 1 );
	glBindTexture( GL_TEXTURE_2D , WaterSimulator::getSingleton()->getPlaneBuffer() );
	glUniform1i( 2 , 1 );
	glActiveTexture( GL_TEXTURE0 + 2 );
	glBindTexture( GL_TEXTURE_2D , _storage_pass.getBufferPtr( 0 ) );
	glUniform1i( 1 , 2 );
	glUniform3fv( 3 , 1 , scene->getCamera()->_v3pos.getArray() );
	glUniform3fv( 3 + 1 , 1 , scene->getCamera()->_v3local_z.getArray() );
	glUniform3fv( 3 + 2 , 1 , cx.getArray() );
	glUniform3fv( 3 + 3 , 1 , cy.getArray() );
	_screen_quad.draw();*/

	return _process_pass.getBufferPtr( 0 );
}
DrawlerDeffered *DrawlerDeffered::getSingleton()
{
	static DrawlerDeffered sng[ 1 ];
	return sng;
}
