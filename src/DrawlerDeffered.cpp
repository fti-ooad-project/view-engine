#include <memory>
#include "../view/DrawlerDeffered.h"
#include "../GlslDefines.h"
#include "../view/FogDrawler.h"
#define LIGHTCASTERS
//#define RENDERWATER
void DrawlerDeffered::bindUniforms( View const &view ) const
{
	glUniform1i( 0 , view._flags );
	if( view._texture_id >= 0 )
	{
		auto const &tex = _textures[ view._texture_id ];
		if( ( view._flags & ShaderMask::MASK_TEXTURED ) && tex.getCount() )
		{
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D_ARRAY , tex.getTexture() );
			glUniform1i( 1 , 0 );///1-layout of the first texture in shader
		}
	}
	if( view._anim_id >= 0 )
	{
		auto const &anim = _animations[ view._anim_id ];
		if( view._flags & ShaderMask::MASK_ANIMATED )
		{
			glUniform1i( 10 , anim._bone_count );
		}
		if( ( view._flags & ShaderMask::MASK_OWN_ANIMATED ) && anim.getCount() )
		{
			glActiveTexture( GL_TEXTURE0 + 1 );
			glBindTexture( GL_TEXTURE_2D_ARRAY , anim.getBufferPtr() );
			glUniform1i( 2 , 1 );///20-location of anim texture
		}
	}
}
uint DrawlerDeffered::loadView( std::string filename , int type , int pref_anim )
{
	auto stream = FileLoader::getStream( filename.c_str() );
	auto mesh = std::unique_ptr< PolyMeshGL >( new PolyMeshGL() );
	mesh->genVboFromMesh( std::move( FileLoader::loadPolyMeshBin( stream , type ) ) );
	_meshes.push_back( std::move( mesh ) );
	View out;
	out._mesh_id = _meshes.size() - 1;
	out._anim_id = -1;
	out._texture_id = -1;
	out._flags = 0;
	int img_count;
	stream->read( ( char* )&img_count , sizeof( int ) );
	if( img_count > 0 )
	{
		out._flags |=
			ShaderMask::MASK_TEXTURED
			| ShaderMask::MASK_TEXTURED_DIF
			| ShaderMask::MASK_TEXTURED_NOR
			| ShaderMask::MASK_TEXTURED_SPE
			;
		std::unique_ptr< std::string[] > fnames( new std::string[ img_count ] );
		ito( img_count )
		{
			int l;
			stream->read( ( char* )&l , sizeof( int ) );
			std::unique_ptr< char[] > fname( new char[ l + 1 ] );
			stream->read( fname.get() , sizeof( char ) * l );
			fname.get()[ l ] = '\0';
			std::string respath( "res/view/images/" );
			respath.append( std::string( fname.get() ) );
			fnames[ i ] = std::move( std::string( respath ) );
		}
		auto textures_data = std::move( FileLoader::loadImage( fnames.get() , img_count ) );
		TextureHolderGL texture;
		texture.init( std::move( textures_data ) , img_count );
		_textures.push_back( std::move( texture ) );
		out._texture_id = _textures.size() - 1;
	}
	if( type != Polymesh::PolyMeshType::BONED_PMESH )
	{
		goto exit;
	}
	out._flags |= ShaderMask::MASK_ANIMATED;
	int anim_count;
	stream->read( ( char * )&anim_count , sizeof( uint ) );
	if( anim_count )
	{
		out._flags |= ShaderMask::MASK_OWN_ANIMATED;
		auto animdata = std::move( FileLoader::loadAnimSetBin( stream , anim_count ) );
		BoneAnimInTexHolderGL animintex;
		animintex.init( std::move( animdata ) , anim_count );
		_animations.push_back( std::move( animintex ) );
		out._anim_id = _animations.size() - 1;
	} else
	{
		out._anim_id = pref_anim;
	}
exit:
	stream->close();
	_view.push_back( out );
	return _view.size() - 1;
}
void DrawlerDeffered::drawInstances( std::vector<InstanceInfo> const *info )
{
	ito( _view.size() )
	{
		_storage_tess_prog.bind();
		bindUniforms( _view[ i ] );
		_storage_prog.bind();
		bindUniforms( _view[ i ] );
		int meshid = _view[ i ]._mesh_id;
		if( info[ 2 * i ].size() > 0 )
		{
			_storage_tess_prog.bind();
			_meshes[ meshid ]->drawInstancedPatches( &info[ i * 2 ][ 0 ] , info[ 2 * i ].size() * sizeof( InstanceInfo ) , info[ 2 * i ].size() );
		}
		if( info[ 2 * i + 1 ].size() > 0 )
		{
			_storage_prog.bind();
			_meshes[ meshid ]->drawInstanced( &info[ i * 2 + 1 ][ 0 ] , info[ 2 * i + 1 ].size() * sizeof( InstanceInfo ) , info[ 2 * i + 1 ].size() );
		}
	}
}
void DrawlerDeffered::drawInstancesToLight( std::vector<InstanceInfo> const *info )
{
	ito( _view.size() )
	{
		int meshid = _view[ i ]._mesh_id;
		bindUniforms( _view[ i ] );
		if( info[ 2 * i ].size() > 0 )
			_meshes[ meshid ]->drawInstanced( &info[ i * 2 ][ 0 ] , info[ 2 * i ].size() * sizeof( InstanceInfo ) , info[ 2 * i ].size() );
		if( info[ 2 * i + 1 ].size() > 0 )
			_meshes[ meshid ]->drawInstanced( &info[ i * 2 + 1 ][ 0 ] , info[ 2 * i + 1 ].size() * sizeof( InstanceInfo ) , info[ 2 * i + 1 ].size() );
	}
}
void DrawlerDeffered::updateRes()
{
	this->_graph_settings = ViewManager::getGraphicSettings();
	_storage_pass.release();
	_storage_pass.init( { { _graph_settings._screen_width , _graph_settings._screen_height } , BufferStoreType::BUFFER_INT , 1 , -1 ,
		false , false , 4 } );
	_process_pass.release();
	_process_pass.init( { { _graph_settings._screen_width , _graph_settings._screen_height } , BufferStoreType::BUFFER_FLOAT , 1 , -1 ,
		false , false , 3 } );
	/*_storage_pass.release();
	_storage_pass.init( PassDesc{ { _resolution.x() , _resolution.y() } , BufferStoreType::BUFFER_INT , 1 , -1 , false , false , 4 } );
	_process_pass.release();
	_process_pass.init( PassDesc{ { _resolution.x() , _resolution.y() } , BufferStoreType::BUFFER_FLOAT , 1 , -1 , false , false , 3 } );
	_water_pass.release();
	_water_pass.init( PassDesc{ { _resolution.x() , _resolution.y() } , BufferStoreType::BUFFER_FLOAT , 1 , -1 , false , false , 3 } );*/
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
	loadView( "res/view/polymodels/monkey.bin" , Polymesh::PolyMeshType::BONED_PMESH );
	{
		auto mesh = new PolyBoxGL();
		mesh->init();
		_meshes.push_back( std::move( std::unique_ptr < PolyMeshGL >( mesh ) ) );
		View boxview;
		boxview._mesh_id = _meshes.size() - 1;
		boxview._anim_id = -1;
		boxview._texture_id = -1;
		boxview._flags = 0;
		_view.push_back( boxview );
	}
	loadView( "res/view/polymodels/tower.bin" , Polymesh::PolyMeshType::STATIC_PMESH );
	loadView( "res/view/polymodels/sword.bin" , Polymesh::PolyMeshType::BONED_PMESH , 0 );


	/*_view.push_back(
		std::move(
		std::unique_ptr < PolyMeshGL
		>( new ComplexPolyMeshGL(
		FileLoader::loadPolyMeshBin(
		FileLoader::getStream(
		"res/view/polymodels/monkey.bin" ) ,
		Polymesh::PolyMeshType::BONED_PMESH ) ) ) ) );
	_view.push_back(
		std::move( std::unique_ptr < PolyMeshGL >( new PolyBoxGL ) ) );
	_view.push_back(
		std::move(
		std::unique_ptr < PolyMeshGL
		>( new ComplexPolyMeshGL(
		FileLoader::loadPolyMeshBin(
		FileLoader::getStream(
		"res/view/polymodels/tower.bin" ) ,
		Polymesh::PolyMeshType::STATIC_PMESH ) ) ) ) );
	_view.push_back(
		std::move(
		std::unique_ptr < PolyMeshGL
		>( new ComplexPolyMeshGL(
		FileLoader::loadPolyMeshBin(
		FileLoader::getStream(
		"res/view/polymodels/sword.bin" ) ,
		Polymesh::PolyMeshType::STATIC_PMESH ) ) ) ) );*/
	std::vector< int > shader_comp{ 4 , 4 , 3 , 3 , 3 , 3 };
	for( auto &i : _meshes )
	{
		i->genInstancedBuffer( 7 , shader_comp );
	}
	ito( LIGHT_CASTER_COUNT )
	{
		_light_dir_passes[ i ].init( { { 1024 , 1024 } ,
			BufferStoreType::BUFFER_FLOAT , 0 , -1 , true , false } );
		//_light_cube_passes[i].init( { { 512 , 512 } , BufferStoreType::BUFFER_FLOAT , 0 , -1 , true , true } );
	}
	_storage_pass.init( { { _graph_settings._screen_width , _graph_settings._screen_height } , BufferStoreType::BUFFER_INT , 1 , -1 ,
		false , false , 4 } );
	_process_pass.init( { { _graph_settings._screen_width , _graph_settings._screen_height } , BufferStoreType::BUFFER_FLOAT , 1 , -1 ,
		false , false , 3 } );
	_water_pass.init( { { 256 , 256 } , BufferStoreType::BUFFER_FLOAT , 1 , -1 ,
		false , false , 3 } );
	_env_tex.init( std::move( FileLoader::loadImage( "res/view/images/sky2.jpg" ) ) , 1 );
	_lightk_tex.init( std::move( FileLoader::loadImage( "res/view/images/lightk.png" ) ) , 1 );
	_lightk_tex.setRepeat( false );
	_blurer.init();
	HeightMapDrawler::getSingleton()->init( 100 , f3( 500.0f , 500.0f , 50.0f ) );
	FogDrawler::getSIngleton()->init( 128 , 128 );
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
	{
		_resolution = res;
		uint asp = std::max( _resolution.x() , _resolution.y() );
		scene->getCamera()->setAspect(
			float( _resolution.x() ) / asp * 3.14f * 0.5f ,
			float( _resolution.y() ) / asp * 3.14f * 0.5f );
	}
	if( this->_graph_settings != ViewManager::getGraphicSettings() )
	{
		//updateRes();
	}
	auto temp_camera = *scene->getCamera();
	std::unique_ptr< std::vector<InstanceInfo>[] > data( new std::vector<InstanceInfo>[ _view.size() * 2 ] );
	float time = _cur_time - floorf( _cur_time );
	{
		for( UnitInstanceState const &ins : scene->getStateVector() )
		{
			//ins._size = _meshes[ _view[ ins._view[ 0 ] ]._mesh_id ]->_size;
			float cam_dist = 1.0f;// f2( ins._pos.x() , ins._pos.y() ).g_dist( f2( temp_camera._v3pos.x() , temp_camera._v3pos.y() ) ) / _meshes[ _view[ ins._view[ 0 ] ]._mesh_id ]->_size.z();
			ins._animstat.update( _dt );
			InstanceInfo instance_info{
				ins._animstat._moment._moment , ins._animstat._moment._last_moment
				, cam_dist , float( ins.selectid ) , int( ins._animstat._moment._mixing )
				, ins._animstat._moment._cur_set , ins._animstat._moment._last_set , int( ins._auto_height ) , ins._pos , ins._look , ins._left , ins._up };
			for( auto const i : ins._view )
			{
				if( cam_dist < 3.0f && _graph_settings._tesselation )
				{
					data[ i * 2 ].push_back( instance_info );
				} else
				{
					//if( !temp_camera.fristrum2d( f2( ins._pos.x() , ins._pos.y() ) ) )
					//	continue;
					if( cam_dist < 15.0f )
					{
						data[ i * 2 + 1 ].push_back( instance_info );
					} else
					{
						data[ 3 ].push_back( instance_info );
					}
				}
			}
		}
	}
	{
		_storage_pass.clear();
		//
		_storage_tess_prog.bind();
		HeightMapDrawler::getSingleton()->bindHeihgtTexture();
		glUniform1f( TIME , time );
		glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE , temp_camera.getViewProj().getPtr() );
		glUniform1i( PASSID , PASS_NORMAL );
		glUniform1i( INSTANSING , 1 );
		glUniform3fv( CAM_POS , 1 , temp_camera._v3pos.getArray() );
		_storage_prog.bind();
		HeightMapDrawler::getSingleton()->bindHeihgtTexture();
		glUniform1f( TIME , time );
		glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE , temp_camera.getViewProj().getPtr() );
		glUniform1i( INSTANSING , 1 );
		glUniform1i( PASSID , PASS_NORMAL );
		glUniform3fv( CAM_POS , 1 , temp_camera._v3pos.getArray() );
		//
		HeightMapDrawler::getSingleton()->bindToDraw();
		glUniform1i( PASSID , PASS_NORMAL );
		glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE , temp_camera.getViewProj().getPtr() );
		glUniform3fv( CAM_POS , 1 , temp_camera._v3pos.getArray() );
		//
		HeightMapDrawler::getSingleton()->draw( true );
		u2 txmpos = ( ( _mpos )* 0.5f + 0.5f ) & f2( _graph_settings._screen_width , _graph_settings._screen_height );
		uint temp[ 4 ];
		glReadBuffer( GL_COLOR_ATTACHMENT0 );
		glReadnPixels( txmpos.x() , txmpos.y() , 1 , 1 , GL_RGBA_INTEGER , GL_UNSIGNED_INT , 4 * sizeof( uint ) , temp );
		_wmpos = temp_camera._v3pos + temp_camera.getCameraRay( _mpos ) * ( float( temp[ 0 ] ) / 100.0f );
		//glPolygonMode( GL_FRONT_AND_BACK , GL_LINE );
		drawInstances( data.get() );
		//glPolygonMode( GL_FRONT_AND_BACK , GL_FILL );
		//glDisable( GL_CULL_FACE );


			//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			/*f4x4 model( 100.0f );
			 _prog[2].bind();
			 glUniform3fv( 11 , 1 , temp_camera._v3pos.getArray() );
			 glUniformMatrix4fv( 6 , 1 , GL_FALSE , model.getPtr() );
			 glActiveTexture( GL_TEXTURE0 );
			 glBindTexture( GL_TEXTURE_2D , WaterSimulator::getSingleton()->getBumpTexture() );
			 glUniform1i( 1 , 0 );
			 glUniform1i( 15 , 0 );
			 glUniform1f( 16 , 0 );
			 glUniform1i( 0 , 0 );
			 _screen_quad.draw();*/
	}
	///lights
#ifdef LIGHTCASTERS
	int caster_dir_light_count = 0 , simple_omni_light_count = 0;
	LightState const *caster_dir_light_ptr[ 1 ]
		, *omni_light_ptr[ 10 ];
	f4x4 dir_lights_viewproj[ LIGHT_CASTER_COUNT ];

	for( LightState const &l : scene->getLightVector() )
	{
		if( l._type == LightSourceType::LIGHT_DIRECT )
		{
			if( caster_dir_light_count > 2 || !l._cast_shadow )
				continue;
			caster_dir_light_ptr[ caster_dir_light_count ] = &l;
			//_light_dir_passes[ caster_dir_light_count ].bind();
			if( _graph_settings._draw_shadows )
			{
				_light_dir_passes[ caster_dir_light_count ].clear();
				dir_lights_viewproj[ caster_dir_light_count ] = Camera::orthographic(
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
			}
			caster_dir_light_count++;
		} else
		{
			omni_light_ptr[ simple_omni_light_count ] = &l;
			simple_omni_light_count++;
		}
	}
#endif
	///water
#ifdef RENDERWATER
	glDisable( GL_CULL_FACE );
	//_storage_tess_prog.bind();
	//WaterSimulator::getSingleton()->bindToRenderSurface();
	//glUniform1i( PASSID , PASS_WATER );
	WaterSimulator::getSingleton()->switchSurfaceBuffer( temp_camera._v3pos );
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
	if( _graph_settings._reflect_water )
	{
		WaterSimulator::getSingleton()->calcReflectionCamera( temp_camera );
		WaterSimulator::getSingleton()->bindToRenderReflection();
		glUniform1i( PASSID , PASS_REFLECT );
		HeightMapDrawler::getSingleton()->draw( true );
		_storage_prog.bind();
		glUniform1i( PASSID , PASS_REFLECT );
		WaterSimulator::getSingleton()->bindToRenderReflection();
		drawInstances( data.get() , false );

		//glEnable( GL_CULL_FACE );
		//HeightMapDrawler::getSingleton()->draw( false );
		
		//glEnable( GL_CULL_FACE );
	}
	WaterSimulator::getSingleton()->calc( _cur_time , _dt );
	//_pass[0].bind();
	WaterSimulator::getSingleton()->clearPlaneBuf();
	WaterSimulator::getSingleton()->bindToRenderPlane( false );
	glUniformMatrix4fv( MAT4X4_VIEWPROJ , 1 , GL_FALSE , temp_camera.getViewProj().getPtr() );
	_screen_quad.draw();
	//glDepthFunc( GL_LESS );
	WaterSimulator::getSingleton()->bindToRenderPlane( true );
	_screen_quad.draw();
	///
#endif
	///
	auto const &select_buff = SelectionDrawler::getSingleton()->process( _storage_pass.getBufferPtr( 0 ) );
	///
	DrawPassGL const *fog_buff;
	if( _graph_settings._draw_fog )
	{
		fog_buff = &_blurer.process(
			FogDrawler::getSIngleton()->process( caster_dir_light_count , _storage_pass.getBufferPtr( 0 ) , _light_dir_passes , dir_lights_viewproj , &temp_camera ).getBufferPtr( 0 )
			, 0.01f
			);
	}
	_process_pass.clear();
	_process_prog.bind();

	_storage_pass.bind( 0 , 0 , 0 );
	_env_tex.bind( 1 , 1 );
	select_buff.bind( 2 , 2 , 0 );
	if( _graph_settings._draw_fog )
	{
		fog_buff->bind( 9 , 10 , 0 );
	}
	glUniform1i( 14 , ( int )_graph_settings._reflect_water );
	/*glActiveTexture( GL_TEXTURE0 + 3 );
	glBindTexture( GL_TEXTURE_2D , _lightk_tex.getTexture() );
	glUniform1i( 8 , 3 );*/

	/*glActiveTexture( GL_TEXTURE0 + 3 );
	glBindTexture( GL_TEXTURE_2D_ARRAY , dynamic_cast< RComplexPolyMeshGL* >( _view[0].get() )->__anim_intex.getBufferPtr() );
	glUniform1i( 3 , 3 );*/
#ifdef RENDERWATER
	glActiveTexture( GL_TEXTURE0 + 4 );
	glBindTexture( GL_TEXTURE_2D , WaterSimulator::getSingleton()->getPlaneBuffer() );
	glUniform1i( 11 , 4 );
	if( _graph_settings._reflect_water )
	{
		glActiveTexture( GL_TEXTURE0 + 6 );
		glBindTexture( GL_TEXTURE_2D , WaterSimulator::getSingleton()->getReflectionPass() );
		glUniform1i( 12 , 6 );
	}
#endif
	glUniform3fv( CAM , 1 , temp_camera._v3pos.getArray() );
	glUniform3fv( CAM + 1 , 1 , temp_camera._v3local_z.getArray() );
	f3 cx = temp_camera._v3local_x * tanf( temp_camera._fovx / 2.0f );
	f3 cy = temp_camera._v3local_y * tanf( temp_camera._fovy / 2.0f );
	glUniform3fv( CAM + 2 , 1 , cx.getArray() );
	glUniform3fv( CAM + 3 , 1 , cy.getArray() );
	glUniformMatrix4fv( 3 , 1 , GL_FALSE , temp_camera.getViewProj().getPtr() );
#ifdef LIGHTCASTERS
	glUniform1i( DLIGHT_COUNT , caster_dir_light_count );
	ito( caster_dir_light_count )
	{
		int lid = DLIGHT + i * 4;
		glUniform3fv( lid , 1 , caster_dir_light_ptr[ i ]->_dir.getArray() );
		glUniform4fv( lid + 1 , 1 , caster_dir_light_ptr[ i ]->_colori.getArray() );
		if( _graph_settings._draw_shadows )
		{
			glUniformMatrix4fv( lid + 2 , 1 , GL_FALSE , dir_lights_viewproj[ i ].getPtr() );
			glActiveTexture( GL_TEXTURE0 + 5 + i );
			glBindTexture( GL_TEXTURE_2D , _light_dir_passes[ i ].getDepthBufferPtr() );
			glUniform1i( lid + 3 , 5 + i );
		}
	}
	glUniform1i( 13 , simple_omni_light_count );
	ito( simple_omni_light_count )
	{
		int lid = 20 + i * 2;
		glUniform4f( lid , omni_light_ptr[ i ]->_pos.x() , omni_light_ptr[ i ]->_pos.y() , omni_light_ptr[ i ]->_pos.z() , omni_light_ptr[ i ]->_size );
		glUniform4fv( lid + 1 , 1 , omni_light_ptr[ i ]->_colori.getArray() );
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
	glUniform3fv( 3 , 1 , temp_camera._v3pos.getArray() );
	glUniform3fv( 3 + 1 , 1 , temp_camera._v3local_z.getArray() );
	glUniform3fv( 3 + 2 , 1 , cx.getArray() );
	glUniform3fv( 3 + 3 , 1 , cy.getArray() );
	_screen_quad.draw();*/

	return _process_pass.getBufferPtr( 0 );
}
f3 DrawlerDeffered::getMousePos( f2 const &mpos )
{
	this->_mpos = mpos;
	return _wmpos;
}
DrawlerDeffered *DrawlerDeffered::getSingleton()
{
	static DrawlerDeffered sng[ 1 ];
	return sng;
}
