#ifndef VIEWENGINEGL_H
#define VIEWENGINEGL_H
#include "view/export/ViewInterface.h"
#include "WindowGL.hpp"
#include "base/REventer.h"
#include "view/3dgl/RDrawableGL.h"
#include "view/3dgl/RViewModelsGL.h"
#include <list>
#include "gui/gl/GUIRendererGL.h"
class Scene3DGL : public Scene3D
{
private:
	RCamera _main_cam;
	REventer _eventer;
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
	REventer *getEventer() override
	{
		return &_eventer;
	}
};
class ViewEngineGL : public ViewManager , public RTimer
{
private:
	std::vector< std::unique_ptr< RPolyMeshGL > > _view;
	RWindowGL win;
	RGraphicProgrammGL _prog;
	RPolyQuadGL _screen_quad;
	RShaderInput _shader_in;
	GUIRendererGL _guimng;
	bool _inited = false;
	void tick( int w , int h )
	{
		if( !_inited )
		{
			//_prog.init( "res/shaders/glsl/screen_quad_frag.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" );
			_prog.init( "res/shaders/glsl/polymesh_frag.glsl" , "res/shaders/glsl/polymesh_vertex.glsl" );
			_screen_quad.init();
			_view.push_back( std::move( std::unique_ptr< RPolyMeshGL >(
				new RComplexPolyMeshGL( RFileLoader::loadPolyMeshBin( RFileLoader::getStream( "res/view/polymodels/monkey.bin" ) , RPolymesh::RPolyMeshType::RBONED_PMESH ) ) ) ) );
			for( std::unique_ptr< RPolyMeshGL > &i : _view )
				i->init();
			_inited = true;
			_view[0]->genInstancedBuffer();
			_guimng.init();
		}
		glBindFramebuffer( GL_FRAMEBUFFER , 0 );
		glClearColor( 0.5f, 0.5f, 0.0f, 0.5f );
		glClearDepth( 1.0f );
		glViewport( 0 , 0 , w , h );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		_shader_in.reset();
		f4x4 model( 1.0f ) , viewproj( RCamera::perpLookUp1x1( f3( 0.0f , 10.0f , 10.0f ) , f3( 0.0f , -0.7f , -0.7f ) , f3( 0.0f , 0.0f , 1.0f ) ) );
		//RTextureHolderGL tex( std::move( std::unique_ptr< RImage[] >( RFileLoader::loadImage( "res/view/images/env_test.jpg" ) ) ) , 1 );
		//tex.init();
		//_shader_in.add( 0 , RShaderInTypes::tex , &( dynamic_cast< RComplexPolyMeshGL* >( _view[0].get() ) )->__anim_intex.__texture_pointer_array[0] );
		updateTime();
		int instancing = 1;
		_shader_in.add( 30 , RShaderInTypes::mat4 , &viewproj );
		int id = _shader_in.add( 15 , RShaderInTypes::ivec1 , &instancing );
		_prog.bind( _shader_in );
		{
			float time = _cur_time - floorf( _cur_time );
			std::vector< InstanceInfo > data;
			xfor( x , 100 )
				xfor( y , 100 )
				{
					f4x4 temp( 1.0f );
					temp( 3 , 0 ) = 2.0f * ( x - 15.0f );
					temp( 3 , 1 ) = 2.0f * ( - y + 5.0f );
					data.push_back( { 0.2 * time , 0.0f , 0 , 0 , 0 , temp } );
				}
			_view[0]->drawInstanced( data );
		}
		instancing = 0;
		_shader_in.add( 6 , RShaderInTypes::mat4 , &model );
		_shader_in.set( 15 , id , RShaderInTypes::ivec1 , &instancing );
		_prog.bind( _shader_in );
		_screen_quad.draw( InstanceInfo() );
		_guimng.drawText( "test" , f2( 0.1f , 0.1f ) , 0.1f );
		//sleep( 0x1000 );
	}
public:
	void init() override
	{
		win.init( [this]( int w , int h ){ tick( w , h ); } );
	}
	void drawScene( Scene3D const *scene ) override
	{

	}
	void drawGUI( GUILayout const *gui ) override
	{
	}
	Scene3D *genScene() override
	{
		return new Scene3DGL();
	}
	void release() override
	{
		win.release();
	}
};
#endif // VIEWENGINEGL_H
