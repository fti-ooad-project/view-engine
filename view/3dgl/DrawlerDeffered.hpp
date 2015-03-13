#ifndef DRAWLERDEFFERED_HPP
#define DRAWLERDEFFERED_HPP
#include "../../base/RBase.h"
#include "view/3dgl/RDrawableGL.h"
#include "view/3dgl/RPassGL.h"
#include "view/3dgl/WaterSimulator.hpp"
#include "view/3dgl/HeighMapDrawler.hpp"
#include "view/3dgl/RViewModelsGL.h"
#include "../export/RCamera.hpp"
#include "../export/ViewInterface.hpp"
class DrawlerDeffered : public RInitable , public RTimer
{
private:
	#define LIGHT_CASTER_COUNT 1
	DrawlerDeffered() = default;
	RGraphicProgrammGL _prog[0x3];
	RDrawPassGL _light_dir_passes[LIGHT_CASTER_COUNT];
	//RDrawPassGL _light_cube_passes[LIGHT_CASTER_COUNT];
	RDrawPassGL  _pass[0x2];
	RTextureHolderGL _env_tex;
	RPolyQuadGL _screen_quad;
	inline void drawInstances( std::vector< InstanceInfo > const *info , bool tess = false )
	{
		if( false )
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
		//_heigmap_drawl.bindToDraw();
		//_heigmap_drawl.draw( true );
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
public:
	std::vector< std::unique_ptr< RPolyMeshGL > > _view;
	void init()
	{
		if( isInited() ) return;
		setInited( true );
		_screen_quad.init();
		_prog[0].init( "res/shaders/glsl/polymesh_frag.glsl" , "res/shaders/glsl/polymesh_tess_vertex.glsl" , "res/shaders/glsl/polymesh_tess_geom.glsl" , "res/shaders/glsl/polymesh_tess_tc.glsl" , "res/shaders/glsl/polymesh_tess_te.glsl" );
		_prog[2].init( "res/shaders/glsl/polymesh_frag.glsl" , "res/shaders/glsl/polymesh_vertex.glsl" , "" );
		_prog[1].init( "res/shaders/glsl/pass1_frag.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" , "" );
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
		ito( LIGHT_CASTER_COUNT )
		{
			_light_dir_passes[i].init( { { 2048 , 2048 } , RBufferStoreType::RBUFFER_FLOAT , 0 , -1 , true , false } );
			//_light_cube_passes[i].init( { { 512 , 512 } , RBufferStoreType::RBUFFER_FLOAT , 0 , -1 , true , true } );
		}
		_pass[0].init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_INT , 1 , -1 , false , false } );
		_pass[1].init( { { 1024 , 1024 } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 , false , false } );
		_env_tex.init( std::move( RFileLoader::loadImage( "res/view/images/ContactSheet_Stallersattel.jpg" ) ) , 1 );
		HeightMapDrawler::getSingleton()->init( 100 , f2( 500.0f , 500.0f ) , f2( 0.0f , 0.0f ) );
		WaterSimulator::getSingleton()->init( _pass[0].getDepthBufferPtr() , f2( 500.0f , 500.0f ) , f3( 0.0f , 0.0f , 0.0f ) );
	}
	void release()
	{
		if( !isInited() ) return;
		setInited( false );
		ito( 3 )
			_prog[i].release();
		ito( 1 )
			_pass[i].release();
		_view.clear();
		_screen_quad.release();
		_env_tex.release();
		HeightMapDrawler::getSingleton()->release();
		WaterSimulator::getSingleton()->release();
	}
	uint draw( Scene3D const *scene , int w , int h )
	{
		updateTime();
		if( !scene ) return 0;
		std::vector< InstanceInfo > data[_view.size()*2];
		float time = _cur_time - floorf( _cur_time );
		scene->getCamera()->setAspect( float( w ) / std::max( w , h ) * RVectorFactory::PI * 0.5f , float( h ) / std::max( w , h ) * RVectorFactory::PI * 0.5f );
		for( RDrawableState const &ins : scene->getStateVector() )
		{
			f4x4 const &m = ins._view[0].model;
			f3 pos = f3( m( 3 , 0 ) , m( 3 , 1 ) , m( 3 , 2 ) );

			float cam_dist = pos.g_dist( scene->getCamera()->_v3pos ) / _view[ins._view[0].view_id]->_size.z() * 2.0f;
			if( cam_dist < 3.0f )
				data[ins._view[0].view_id*2].push_back( { 0.2 * time , 0.0f , cam_dist , 0 , 0 , 0 , m } );
			else
			{
				if( !scene->getCamera()->fristrum( pos , _view[ins._view[0].view_id]->_size.z() ) ) continue;

				if( cam_dist < 50.0f )
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
		_pass[0].clear();
		_prog[0].bind();
		glUniform1f( 7 , time );
		glUniformMatrix4fv( 30 , 1 , GL_FALSE , scene->getCamera()->getViewProj().getPtr() );
		glUniform1i( 17 , 0 );
		glUniform1i( 15 , 1 );
		glUniform3fv( 11 , 1 , scene->getCamera()->_v3pos.getArray() );
		_prog[2].bind();
		glUniform1f( 7 , time );
		glUniformMatrix4fv( 30 , 1 , GL_FALSE , scene->getCamera()->getViewProj().getPtr() );
		glUniform1i( 15 , 1 );
		glUniform1i( 17 , 0 );
		glUniform3fv( 11 , 1 , scene->getCamera()->_v3pos.getArray() );
		//
		//_heigmap_drawl.bindToDraw();
		glUniform1i( 17 , 0 );
		glUniform1i( 0 , ShaderMask::MASK_TEXTURED | ShaderMask::MASK_TEXTURED_DIF );
		glUniformMatrix4fv( 30 , 1 , GL_FALSE , scene->getCamera()->getViewProj().getPtr() );
		glUniform3fv( 11 , 1 , scene->getCamera()->_v3pos.getArray() );

		drawInstances( data , true );
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
		RLightState const *caster_dir_light_ptr[LIGHT_CASTER_COUNT] , *caster_omni_light_ptr[LIGHT_CASTER_COUNT];
		f4x4 dir_lights_viewproj[LIGHT_CASTER_COUNT];

		for( RLightState const &l : scene->getLightVector() )
		{
			if( l._type == RLightSourceType::RLIGHT_DIRECT )
			{
				if( caster_dir_light_count > 2 || !l._cast_shadow ) continue;
				caster_dir_light_ptr[caster_dir_light_count] = &l;
				_light_dir_passes[caster_dir_light_count].bind();
				_light_dir_passes[caster_dir_light_count].clear();

				dir_lights_viewproj[caster_dir_light_count] = RCamera::orthographic( l._pos , l._dir , f3( 0.0f , 0.0f , 1.0f ) );

				_prog[2].bind();
				glUniform1i( 17 , 1 );
				glUniformMatrix4fv( 30 , 1 , GL_FALSE , dir_lights_viewproj[caster_dir_light_count].getPtr() );
				drawInstancesToLight( data );

				//_heigmap_drawl.bindToDraw();
				glUniform1i( 17 , 1 );
				glUniformMatrix4fv( 30 , 1 , GL_FALSE , dir_lights_viewproj[caster_dir_light_count].getPtr() );
				//_heigmap_drawl.draw( true );

				caster_dir_light_count++;
			}else
			{
				if( caster_omni_light_count > 2 ) continue;
				caster_omni_light_count++;
			}
		}
		///water
		/*if( scene )
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
			glUniformMatrix4fv( 30 , 1 , GL_FALSE , scene->getCamera()->getViewProj().getPtr() );
			glActiveTexture( GL_TEXTURE0 );
			glBindTexture( GL_TEXTURE_2D , WaterSimulator::getSingleton()->getBumpTexture() );
			glUniform1i( 1 , 0 );
			_screen_quad.draw();
		}*/
		_pass[1].clear();
		_prog[1].bind();

		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D , _pass[0].getBufferPtr( 0 ) );
		glUniform1i( 0 , 0 );
		glActiveTexture( GL_TEXTURE0 + 1 );
		glBindTexture( GL_TEXTURE_2D , _env_tex.getTexture( 0 ) );
		glUniform1i( 1 , 1 );
		//glActiveTexture( GL_TEXTURE0 + 4 );
		//glBindTexture( GL_TEXTURE_2D , WaterSimulator::getSingleton()->getPlaneBuffer() );
		//glUniform1i( 11 , 4 );
		glUniform1i( 10 , caster_dir_light_count );
		glUniform3fv( 4 , 1 , scene->getCamera()->_v3pos.getArray() );
		glUniform3fv( 5 , 1 , scene->getCamera()->_v3local_z.getArray() );
		f3 cx = scene->getCamera()->_v3local_x * tanf( scene->getCamera()->_fovx / 2.0f );
		f3 cy = scene->getCamera()->_v3local_y * tanf( scene->getCamera()->_fovy / 2.0f );
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
		return _pass[1].getBufferPtr( 0 );
	}
	static DrawlerDeffered *getSingleton()
	{
		static DrawlerDeffered sng[1];
		return sng;
	}
};
#endif // DRAWLERDEFFERED_HPP
