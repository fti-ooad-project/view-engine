#ifndef VIEWENGINEGL_H
#define VIEWENGINEGL_H
#include "view/export/ViewInterface.hpp"
#include "WindowGL.hpp"
#include "base/REventer.h"
#include "view/3dgl/RDrawableGL.h"
#include "view/3dgl/RViewModelsGL.h"
#include <list>
#include "gui/gl/GUIRendererGL.hpp"
#include "3dgl/DrawlerDeffered.hpp"
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
	RCamera const *getCamera() const override
	{
		return &_main_cam;
	}
	RCamera *getCamera() override
	{
		return &_main_cam;
	}
	std::vector< RDrawableState > const &getStateVector() const override
	{
		return _instances;
	}
	std::vector< RLightState > const &getLightVector() const override
	{
		return _lights;
	}
};
class ViewEngineGL : public ViewManager
{
private:
	RWindowGL win;
	RGraphicProgrammGL _quad_prog;
	bool _tess = true;
	RPolyQuadGL _screen_quad;
	REventer _eventer;
	//GUIRendererGL _guimng;
	bool _inited = false;
	Scene3DGL const *_cur_scene = nullptr;
	void tick( int w , int h )
	{
		if( !_inited )
		{
			DrawlerDeffered::getSingleton()->init();
			_screen_quad.init();
			_inited = true;
			_quad_prog.init( "res/shaders/glsl/screen_quad_frag.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" );
			//_guimng.init();
			//_guimng.genText( "start game" );
		}
		uint draw = DrawlerDeffered::getSingleton()->draw( _cur_scene , w , h );
		glBindFramebuffer( GL_FRAMEBUFFER , 0 );
		glClearColor( 0.0f, 0.5f, 0.0f, 0.0f );
		glClearDepth( 1.0f );
		glViewport( 0 , 0 , w , h );
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		_quad_prog.bind();
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D , draw );
		glUniform1i( 0 , 0 );
		_screen_quad.draw();
		//_guimng.drawPanel( f2( 0.0f , 0.0f ) , f2( 0.2f , 0.2f ) + 0.1f * f2( sin( _cur_time ) , cos( _cur_time ) ) , 0 );
		//_guimng.drawText( 0 , f2( 0.0f , 0.0f ) , f2( 0.2f , 0.1f ) );
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
		DrawlerDeffered::getSingleton()->release();
		_quad_prog.release();
		_screen_quad.release();

		//WaterSimulator::getSingleton()->release();
	}
};
#endif // VIEWENGINEGL_H
