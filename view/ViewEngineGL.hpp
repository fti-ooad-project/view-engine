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
	std::vector< std::unique_ptr< RPolyMeshGL > > _view;
	RWindowGL win;
	RGraphicProgrammGL _quad_prog;
	RGraphicProgrammGL _prog[0x3];
	RDrawPassGL  _pass[0x1];
	int tri = 0;
	RPolyQuadGL _screen_quad;
	REventer _eventer;
	f4x4 viewproj = RCamera::perpLookUp1x1( f3( 0.0f , 10.0f , 10.0f ) , f3( 0.0f , -0.7f , -0.7f ) , f3( 0.0f , 0.0f , 1.0f ) );
	//GUIRendererGL _guimng;
	bool _inited = false;
	Scene3DGL const *_cur_scene = nullptr;
	inline void drawInstances( std::vector< InstanceInfo > const *info , bool tess = false )
	{
		if( tri )
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );glLineWidth( 0.01f );
		}
		if( tess )
		{
			ito( _view.size() )
			{
				if( info[i].size() < 1 ) continue;
				_view[i]->drawInstancedPatches( info[i] );
			}
		}else
		{
			ito( _view.size() )
			{
				if( info[i].size() < 1 ) continue;
				_view[i]->drawInstanced( info[i] );
			}
		}
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
	void tick( int w , int h )
	{
		if( !_inited )
		{
			_quad_prog.init( "res/shaders/glsl/screen_quad_frag.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" );
			_pass[0].init( { { 512 , 512 } , RBufferStoreType::RBUFFER_FLOAT , 4 , -1 , false , false } );
			_prog[0].init( "res/shaders/glsl/polymesh_frag.glsl" , "res/shaders/glsl/polymesh_tess_vertex.glsl" , "res/shaders/glsl/polymesh_tess_geom.glsl" , "res/shaders/glsl/polymesh_tess_tc.glsl" , "res/shaders/glsl/polymesh_tess_te.glsl" );
			_prog[2].init( "res/shaders/glsl/polymesh_frag.glsl" , "res/shaders/glsl/polymesh_vertex.glsl" , "" );
			_prog[1].init( "res/shaders/glsl/pass1_frag.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" , "" );
			_screen_quad.init();
			_view.push_back( std::move( std::unique_ptr< RPolyMeshGL >(
				new RComplexPolyMeshGL( RFileLoader::loadPolyMeshBin( RFileLoader::getStream( "res/view/polymodels/monkey.bin" ) , RPolymesh::RPolyMeshType::RBONED_PMESH ) ) ) ) );
			for( std::unique_ptr< RPolyMeshGL > &i : _view )
			{
				i->init();
				i->genInstancedBuffer();
			}
			_inited = true;
			//_guimng.init();
			//_guimng.genText( "start game" );
		}
		std::vector< InstanceInfo > data[_view.size()];
		if( _cur_scene )
		{
			float time = _cur_time - floorf( _cur_time );
			for( RDrawableState const &ins : _cur_scene->_instances )
			{
				data[std::get< 0 >( ins._view[0] )].push_back( { 0.2 * time , 0.0f , 0 , 0 , 0 , std::get< 1 >( ins._view[0] ) } );
			}
		}
		int instancing = 1;
		{
			/*auto rend = TextRenderer::getSingleton();
		//rend->init();
		//RTextureHolderGL tex( rend->renderText( "hello" ) , 1 );
		//std::move( std::unique_ptr< RImage[] >( RFileLoader::loadImage( "res/view/images/env_test.jpg" ) ) ) , 1 );
		//tex.init();
		//_shader_in.add( 0 , RShaderInTypes::tex , &tex.__texture_pointer_array[0] );*/
			updateTime();
			_prog[0].bind();
			if( _cur_scene )
				glUniformMatrix4fv( 30 , 1 , GL_FALSE , _cur_scene->_main_cam.getViewProj().getPtr() );
			else
				glUniformMatrix4fv( 30 , 1 , GL_FALSE , viewproj.getPtr() );

			glUniform1i( 15 , 1 );
			glUniform1i( 12 , 1 );
			glUniform3fv( 11 , 1 , _cur_scene->_main_cam._v3pos.getArray() );
			//
			_pass[0].bind();
			_pass[0].clear();
			drawInstances( data , true );
			instancing = 0;
			f4x4 model( 100.0f );
			_prog[2].bind();
			if( _cur_scene )
				glUniformMatrix4fv( 30 , 1 , GL_FALSE , _cur_scene->_main_cam.getViewProj().getPtr() );
			else
				glUniformMatrix4fv( 30 , 1 , GL_FALSE , viewproj.getPtr() );
			glUniformMatrix4fv( 6 , 1 , GL_FALSE , model.getPtr() );
			glUniform1i( 15 , 0 );
			glUniform1i( 0 , 0 );
			_screen_quad.draw();
		}
		glBindFramebuffer( GL_FRAMEBUFFER , 0 );
		glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
		glClearDepth( 1.0f );
		glViewport( 0 , 0 , w , h );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		_quad_prog.bind();
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D , _pass[0].getBufferPtr( 3 ) );
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
	}
};
#endif // VIEWENGINEGL_H
