#include "../view/ViewEngineGL.h"
Scene3DGL::Scene3DGL()
{
}
ViewTypes::RDrawInstancePTR Scene3DGL::genInstance()
{
	_instances.push_back( UnitInstanceState() );
	return _instances.size() - 1;
}
UnitInstanceState *Scene3DGL::getInstanceStatePtr( ViewTypes::RDrawInstancePTR i )
{
	return &_instances[ i ];
}
ViewTypes::RLightSourcePTR Scene3DGL::genLight()
{
	_lights.push_back( LightState() );
	return _lights.size() - 1;
}
LightState *Scene3DGL::getLightStatePtr( ViewTypes::RLightSourcePTR i )
{
	return &_lights[ i ];
}
RCamera const *Scene3DGL::getCamera() const
{
	return &_main_cam;
}
RCamera *Scene3DGL::getCamera()
{
	return &_main_cam;
}
std::vector< UnitInstanceState > const &Scene3DGL::getStateVector() const
{
	return _instances;
}
std::vector< LightState > const &Scene3DGL::getLightVector() const
{
	return _lights;
}
void ViewEngineGL::tick( int w , int h )
{
	if( !_inited )
	{
		
		DrawlerDeffered::getSingleton()->init();
		_screen_quad.init();
		_inited = true;
		_quad_prog.init( "res/shaders/glsl/screen_quad_frag.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" );
		_guimng.init();
		//_guimng.genText( "start game" );
	}
	_resolution = i2( w , h );
	uint draw = DrawlerDeffered::getSingleton()->draw( _cur_scene , _resolution );
	glBindFramebuffer( GL_FRAMEBUFFER , 0 );
	glClearColor( 0.0f , 0.0f , 0.0f , 0.0f );
	glClearDepth( 1.0f );
	glViewport( 0 , 0 , w , h );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	_quad_prog.bind();
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , draw );
	glUniform1i( 0 , 0 );
	_screen_quad.draw();
	//if( _gui )
		_guimng.renderLayout( w , h , _gui );
	//_guimng.drawPanel( f2( 0.0f , 0.0f ) , f2( 0.3f , 0.2f ) , 0 );
	//_guimng.drawText( 0 , f2( 0.0f , 0.0f ) , f2( 0.2f , 0.1f ) );
}
void ViewEngineGL::updateRes()
{

}
void ViewEngineGL::init()
{
	if( isInited() ) return;
	setInited( true );
	win.init( [ this ]( int w , int h ) { tick( w , h ); } , [ this ]() { this->release(); } , &_eventer );
	_eventer.start();
}
void ViewEngineGL::setScene( Scene3D const *scene )
{
	_cur_scene = dynamic_cast< Scene3DGL const * >( scene );
}
void ViewEngineGL::setResolution( int w , int h )
{
	_resolution = i2( w , h );
}
void ViewEngineGL::setGUI( GUILayout const *gui )
{
	_gui = gui;
}
Scene3D *ViewEngineGL::genScene()
{
	return new Scene3DGL();
}
Scene3D  const *ViewEngineGL::getScene()
{
	return static_cast< Scene3D const* >( _cur_scene );
}
REventer *ViewEngineGL::getEventer()
{
	return &_eventer;
}
void ViewEngineGL::release()
{
	if( !isInited() ) return;
	setInited( false );
	DrawlerDeffered::getSingleton()->release();
	_quad_prog.release();
	_screen_quad.release();
	_eventer.release();
	//WaterSimulator::getSingleton()->release();
}