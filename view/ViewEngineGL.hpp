#ifndef VIEWENGINEGL_H
#define VIEWENGINEGL_H
#include "view/export/ViewInterface.hpp"
#include "WindowGL.hpp"
#include "base/REventer.h"
#include "view/3dgl/RDrawableGL.h"
#include "view/3dgl/RViewModelsGL.h"
#include <list>
#include "gui/gl/GUIRendererGL.hpp"
#include "view/3dgl/RPassGL.h"
#include "3dgl/WaterSimulator.hpp"
class Scene3DGL : public Scene3D
{
public:
	RCamera _main_cam;
	std::vector< RDrawableState > _instances;
	std::vector< RLightState > _lights;
public:
	Scene3DGL()
	{
	}
	ViewTypes::RDrawInstancePTR genInstance() override
	{
		_instances.push_back( RDrawableState() );
		return _instances.size() - 1;
	}
	RDrawableState *getInstanceStatePtr( ViewTypes::RDrawInstancePTR i ) override
	{
		return &_instances[i];
	}
	ViewTypes::RLightSourcePTR genLight() override
	{
		_lights.push_back( RLightState() );
		return _lights.size() - 1;
	}
	RLightState *getLightStatePtr( ViewTypes::RLightSourcePTR i ) override
	{
		return &_lights[i];
	}
	RCamera *getCamera() override
	{
		return &_main_cam;
	}
};
class ViewEngineGL : public ViewManager , public RTimer
{
private:
	#define LIGHT_CASTER_COUNT 3
	std::vector< std::unique_ptr< RPolyMeshGL > > _view;
	RWindowGL win;
	RGraphicProgrammGL _quad_prog;
	RGraphicProgrammGL _prog[0x3];
	RGraphicProgrammGL _light_cube_prog;
	RGraphicProgrammGL _light_dir_prog;
	RGraphicProgrammGL _skybox_shader;

	RDrawPassGL _light_dir_passes[LIGHT_CASTER_COUNT];
	RDrawPassGL _light_cube_passes[LIGHT_CASTER_COUNT];
	RDrawPassGL  _pass[0x2];

	bool _tri = false;
	bool _tess = true;
	RPolyQuadGL _screen_quad;
	REventer _eventer;
	//f4x4 viewproj = RCamera::perpLookUp1x1( f3( 0.0f , 10.0f , 10.0f ) , f3( 0.0f , -0.7f , -0.7f ) , f3( 0.0f , 0.0f , 1.0f ) );
	//GUIRendererGL _guimng;
	bool _inited = false;
	Scene3DGL const *_cur_scene = nullptr;
	inline void drawInstances( std::vector< InstanceInfo > const *info , bool tess = false )
	{
		if( _tri )
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
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
				if( info[2*i].size() > 0 )
				{
					_prog[0].bind();
					_view[i]->drawInstancedPatches( info[i*2] );
				}
				if( info[2*i+1].size() > 0 )
				{
					_prog[2].bind();
					_view[i]->drawInstanced( info[i*2+1] );
				}
			}else
			{
				_prog[2].bind();
				if( info[2*i].size() > 0 )
				{
					_view[i]->drawInstanced( info[i*2] );
				}
				if( info[2*i+1].size() > 0 )
				{
					_view[i]->drawInstanced( info[i*2+1] );
				}
			}
		}
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
	inline void drawInstancesToLight( std::vector< InstanceInfo > const *info )
	{
		ito( _view.size() )
		{
			if( info[2*i].size() > 0 )
				_view[i]->drawInstanced( info[i*2] );
			if( info[2*i+1].size() > 0 )
				_view[i]->drawInstanced( info[i*2+1] );
		}
	}
	void tick( int w , int h )
	{
		if( !_inited )
		{
			ito( LIGHT_CASTER_COUNT )
			{
				_light_dir_passes[i].init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_FLOAT , 0 , -1 , true , false } );
				_light_cube_passes[i].init( { { 512 , 512 } , RBufferStoreType::RBUFFER_FLOAT , 0 , -1 , true , true } );
			}

			_quad_prog.init( "res/shaders/glsl/screen_quad_frag.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" );
			_pass[0].init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_INT , 1 , -1 , false , false } );
			_pass[1].init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 , false , false } );
			_prog[0].init( "res/shaders/glsl/polymesh_frag.glsl" , "res/shaders/glsl/polymesh_tess_vertex.glsl" , "res/shaders/glsl/polymesh_tess_geom.glsl" , "res/shaders/glsl/polymesh_tess_tc.glsl" , "res/shaders/glsl/polymesh_tess_te.glsl" );
			_prog[2].init( "res/shaders/glsl/polymesh_frag.glsl" , "res/shaders/glsl/polymesh_vertex.glsl" , "" );
			_prog[1].init( "res/shaders/glsl/pass1_frag.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" , "" );
			_skybox_shader.init( "res/shaders/glsl/skybox_fragment.glsl" , "res/shaders/glsl/skybox_vertex.glsl" );
			_light_cube_prog.init( "res/shaders/glsl/depth_allpass_fragment.glsl" , "res/shaders/glsl/depth_cubepass_vertex.glsl" , "res/shaders/glsl/depth_cubepass_geometry.glsl" );
			_light_dir_prog.init( "res/shaders/glsl/depth_allpass_fragment.glsl" , "res/shaders/glsl/depth_dirpass_vertex.glsl" );
			_screen_quad.init();
			_view.push_back( std::move( std::unique_ptr< RPolyMeshGL >(
				new RComplexPolyMeshGL( RFileLoader::loadPolyMeshBin( RFileLoader::getStream( "res/view/polymodels/monkey.bin" ) , RPolymesh::RPolyMeshType::RBONED_PMESH ) ) ) ) );
			_view.push_back( std::move( std::unique_ptr< RPolyMeshGL >( new RPolyBoxGL ) ) );
			_view.push_back( std::move( std::unique_ptr< RPolyMeshGL >(
				new RComplexPolyMeshGL( RFileLoader::loadPolyMeshBin( RFileLoader::getStream( "res/view/polymodels/tower.bin" ) , RPolymesh::RPolyMeshType::RSTATIC_PMESH ) ) ) ) );
			for( std::unique_ptr< RPolyMeshGL > &i : _view )
			{
				i->init();
				i->genInstancedBuffer();
			}
			_inited = true;
			WaterSimulator::getSingleton()->init( _pass[0].getDepthBufferPtr() );
			//_guimng.init();
			//_guimng.genText( "start game" );
		}
		std::vector< InstanceInfo > data[_view.size()*2];
		float time = _cur_time - floorf( _cur_time );
		if( _cur_scene )
		{
			_cur_scene->_main_cam.setAspect( float( w ) / std::max( w , h ) * RVectorFactory::PI * 0.5f , float( h ) / std::max( w , h ) * RVectorFactory::PI * 0.5f );
			for( RDrawableState const &ins : _cur_scene->_instances )
			{
				f4x4 const &m = ins._view[0].model;
				f3 pos = f3( m( 3 , 0 ) , m( 3 , 1 ) , m( 3 , 2 ) );

				float cam_dist = pos.g_dist( _cur_scene->_main_cam._v3pos ) / _view[ins._view[0].view_id]->_size.z() * 2.0f;
				if( cam_dist < 10.0f && _tess )
					data[ins._view[0].view_id*2].push_back( { 0.2 * time , 0.0f , cam_dist , 0 , 0 , 0 , m } );
				else
				{
					if( !_cur_scene->_main_cam.fristrum( pos , _view[ins._view[0].view_id]->_size.z() ) ) continue;

					if( cam_dist < 130.0f )
						data[ins._view[0].view_id*2 + 1].push_back( { 0.2 * time , 0.0f , cam_dist , 0 , 0 , 0 , m } );
					else
					{
						f4x4 mn = m;
						mn.scale( _view[ins._view[0].view_id]->_size );
						data[3].push_back( { 0.2 * time , 0.0f , cam_dist , 0 , 0 , 0 , mn } );
					}
				}
			}
			if( time < 0.01f )
				LOG << 1.0f / _dt << "\n";
		}
		if( _cur_scene )
		{
			/*auto rend = TextRenderer::getSingleton();
		//rend->init();
		//RTextureHolderGL tex( rend->renderText( "hello" ) , 1 );
		//std::move( std::unique_ptr< RImage[] >( RFileLoader::loadImage( "res/view/images/env_test.jpg" ) ) ) , 1 );
		//tex.init();
		//_shader_in.add( 0 , RShaderInTypes::tex , &tex.__texture_pointer_array[0] );*/
			updateTime();
			_pass[0].clear();
			_prog[0].bind();
			glUniform1f( 7 , time );
			glUniformMatrix4fv( 30 , 1 , GL_FALSE , _cur_scene->_main_cam.getViewProj().getPtr() );

			glUniform1i( 15 , 1 );
			glUniform1i( 12 , 1 );
			glUniform3fv( 11 , 1 , _cur_scene->_main_cam._v3pos.getArray() );
			_prog[2].bind();
			glUniform1f( 7 , time );
			glUniformMatrix4fv( 30 , 1 , GL_FALSE , _cur_scene->_main_cam.getViewProj().getPtr() );
			glUniform1i( 15 , 1 );
			glUniform1i( 12 , 1 );
			glUniform3fv( 11 , 1 , _cur_scene->_main_cam._v3pos.getArray() );
			//

			drawInstances( data , true );

			f4x4 model( 100.0f );
			_prog[2].bind();
			glUniform3fv( 11 , 1 , _cur_scene->_main_cam._v3pos.getArray() );
			glUniformMatrix4fv( 6 , 1 , GL_FALSE , model.getPtr() );
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D , WaterSimulator::getSingleton()->getBumpTexture() );
			glUniform1i( 1 , 0 );
			glUniform1i( 15 , 0 );
			glUniform1f( 16 , 0 );
			glUniform1i( 0 , 0 );
			_screen_quad.draw();
		}
		///lights
		int caster_dir_light_count = 0 , caster_omni_light_count = 0;
		RLightState const *caster_dir_light_ptr[LIGHT_CASTER_COUNT] , *caster_omni_light_ptr[LIGHT_CASTER_COUNT];
		f4x4 dir_lights_viewproj[LIGHT_CASTER_COUNT];
		if( _cur_scene )
		{
			for( RLightState const &l : _cur_scene->_lights )
			{
				if( l._type == RLightSourceType::RLIGHT_DIRECT )
				{
					if( caster_dir_light_count > 2 || !l._cast_shadow ) continue;
					caster_dir_light_ptr[caster_dir_light_count] = &l;
					_light_dir_passes[caster_dir_light_count].bind();
					_light_dir_passes[caster_dir_light_count].clear();
					_light_dir_prog.bind();
					glUniform1i( 15 , 1 );
					dir_lights_viewproj[caster_dir_light_count] = RCamera::orthographic( l._pos , l._dir , f3( 0.0f , 0.0f , 1.0f ) );
					glUniformMatrix4fv( 30 , 1 , GL_FALSE , dir_lights_viewproj[caster_dir_light_count].getPtr() );
					drawInstancesToLight( data );
					//dir_lights_viewproj[caster_dir_light_count].print();
					caster_dir_light_count++;
				}else
				{
					if( caster_omni_light_count > 2 ) continue;
					caster_omni_light_count++;
				}
			}
		}
		///water
		/*if( _cur_scene )
		{
			WaterSimulator::getSingleton()->bindToRenderSurface();
			glUniform1f( 7 , time );
			f4x4 water_viewproj = RCamera::orthographic( f3( 0.0f , 0.0f , -1.0f ) , f3( 0.0f , 0.0f , 1.0f ) , f3( 0.0f , 1.0f , 0.0f ) );
			glUniformMatrix4fv( 30 , 1 , GL_FALSE , water_viewproj.getPtr() );
			glUniform1i( 15 , 1 );
			glUniform1i( 12 , 1 );
			drawInstancesToLight( data );
			WaterSimulator::getSingleton()->calc();
			//_pass[0].bind();
			WaterSimulator::getSingleton()->bindToRenderPlane();
			glUniformMatrix4fv( 30 , 1 , GL_FALSE , _cur_scene->_main_cam.getViewProj().getPtr() );
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D , WaterSimulator::getSingleton()->getBumpTexture() );
			glUniform1i( 1 , 0 );
			_screen_quad.draw();
		}*/
		_pass[1].clear();
		_prog[1].bind();
		{
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D , _pass[0].getBufferPtr( 0 ) );
			glUniform1i( 0 , 0 );
			//glActiveTexture( GL_TEXTURE0 + 4 );
			//glBindTexture( GL_TEXTURE_2D , WaterSimulator::getSingleton()->getPlaneBuffer() );
			//glUniform1i( 11 , 4 );
			glUniform1i( 10 , caster_dir_light_count );
			glUniform3fv( 4 , 1 , _cur_scene->_main_cam._v3pos.getArray() );
			glUniform3fv( 5 , 1 , _cur_scene->_main_cam._v3local_z.getArray() );
			f3 cx = _cur_scene->_main_cam._v3local_x * tanf( _cur_scene->_main_cam._fovx / 2.0f );
			f3 cy = _cur_scene->_main_cam._v3local_y * tanf( _cur_scene->_main_cam._fovy / 2.0f );
			glUniform3fv( 6 , 1 , cx.getArray() );
			glUniform3fv( 7 , 1 , cy.getArray() );
			ito( caster_dir_light_count )
			{
				int lid = 32 + i * 4;
				glUniform3fv( lid , 1 , caster_dir_light_ptr[i]->_dir.getArray() );
				glUniform4fv( lid + 1 , 1 , caster_dir_light_ptr[i]->_colori.getArray() );
				glUniformMatrix4fv( lid + 2 , 1 , GL_FALSE , dir_lights_viewproj[i].getPtr() );
				glActiveTexture( GL_TEXTURE0 + 5 + i );
				glBindTexture( GL_TEXTURE_2D , _light_dir_passes[i].getDepthBufferPtr() );
				glUniform1i( lid + 3 , 5 + i );
			}
			_screen_quad.draw();
		}
		glBindFramebuffer( GL_FRAMEBUFFER , 0 );
		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		glClearDepth( 1.0f );
		glViewport( 0 , 0 , w , h );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		_quad_prog.bind();
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D , _pass[1].getBufferPtr( 0 ) );
		glUniform1i( 0 , 0 );
		_screen_quad.draw();


		//_guimng.drawPanel( f2( 0.0f , 0.0f ) , f2( 0.2f , 0.2f ) + 0.1f * f2( sin( _cur_time ) , cos( _cur_time ) ) , 0 );
		//_guimng.drawText( 0 , f2( 0.0f , 0.0f ) , f2( 0.2f , 0.1f ) );
		//sleep( 0x1000 );
	}
public:
	void init() override
	{
		if( isInited() ) return;
		setInited( true );
		win.init( [this]( int w , int h ){ tick( w , h ); } , [this](){ this->release(); } , &_eventer );
	}
	void setScene( Scene3D const *scene ) override
	{
		_cur_scene = dynamic_cast< Scene3DGL const * >( scene );
	}
	void setGUI( GUILayout const *gui ) override
	{
	}
	Scene3D *genScene() override
	{
		return new Scene3DGL();
	}
	REventer *getEventer() override
	{
		return &_eventer;
	}
	void release() override
	{
		if( !isInited() ) return;
		setInited( false );
		//_guimng.release();
		_quad_prog.release();
		_screen_quad.release();
		ito( 3 )
			_prog[i].release();
		ito( 1 )
			_pass[i].release();
		_view.clear();
		WaterSimulator::getSingleton()->release();
	}
};
#endif // VIEWENGINEGL_H
