#include "../view/ViewEngineGL.h"
f4 selection_box;
Scene3DGL::Scene3DGL()
{
}
uint Scene3DGL::genInstance()
{
	_instances.push_back( UnitInstanceState() );
	return _instances.size() - 1;
}
UnitInstanceState *Scene3DGL::getInstanceStatePtr( uint i )
{
	return &_instances[ i ];
}
uint Scene3DGL::genLight()
{
	_lights.push_back( LightState() );
	return _lights.size() - 1;
}
LightState *Scene3DGL::getLightStatePtr( uint i )
{
	return &( _lights[ i ] );
}
Camera const *Scene3DGL::getCamera() const
{
	return &_main_cam;
}
Camera *Scene3DGL::getCamera()
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
uint Scene3DGL::getSelected( f2 const &mpos ) const
{
	float min = 9999999.0f;
	float min2d = 99999999.9f;
	int mindx = -1;
	for( int i = 0; i < _instances.size(); i ++ )
	{
		float dp = ( _instances[i]._pos - _main_cam._v3pos ).g_mod();
		f4 scr_pos = f4( _instances[ i ]._pos + f3( f2( 0.0f ) , _instances[ i ]._size.z() ) , 1.0f ) * _main_cam.getViewProj();
		scr_pos /= scr_pos.w();
		float tmp = scr_pos.xy().g_dist2( mpos );
		if( isIn( mpos , scr_pos.xy() , f2( _instances[i]._size.x() / dp , _instances[ i ]._size.z() / dp * _main_cam._v3local_y.z() ) ) && scr_pos.z() < min && tmp < min2d )
		{
			min = scr_pos.z();
			min2d = tmp;
			mindx = i;
		}
	}
	return mindx;
}
std::vector< uint > Scene3DGL::getSelected( f2 const &center , f2 const &dr ) const
{
	std::vector< uint > out;
	for( int i = 0; i < _instances.size(); i++ )
	{
		f4 scr_pos = f4( _instances[ i ]._pos + f3( f2( 0.0f ) , _instances[ i ]._size.z() ) , 1.0f ) * _main_cam.getViewProj();
		scr_pos /= scr_pos.w();
		if( isIn( scr_pos.xy() , center , dr ) )
			out.push_back( i );
	}
	return out;
}
void ViewEngineGL::tick( int w , int h )
{
	if( !_inited )
	{
		
		DrawlerDeffered::getSingleton()->init();
		_screen_quad.init();
		_inited = true;
		_quad_prog.init( "res/shaders/glsl/screen_quad_frag.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" );
		GUIRendererGL::getSingleton()->init();
		//GUIRendererGL::getSingleton()->genText( "start game" );
	}
	i2 _resolution = i2( w , h );
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
	glUniform4fv( 1 , 1 , selection_box.getArray() );
	_screen_quad.draw();
	if( _gui )
		GUIRendererGL::getSingleton()->renderLayout( w , h , _gui );
	//GUIRendererGL::getSingleton()->drawPanel( f2( 0.0f , 0.0f ) , f2( 0.3f , 0.2f ) , 0 );
	//GUIRendererGL::getSingleton()->drawText( 0 , f2( 0.0f , 0.0f ) , f2( 0.2f , 0.1f ) );
}
void ViewEngineGL::init()
{
	if( isInited() ) return;
	setInited( true );
	win.init( [ this ]( int w , int h ) { tick( w , h ); } , [ this ]() { this->release(); } , &_eventer , false , "OOOD PROJ" );
	_eventer.start();
}
void ViewEngineGL::setScene( Scene3D const *scene )
{
	_cur_scene = dynamic_cast< Scene3DGL const * >( scene );
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
Eventer *ViewEngineGL::getEventer()
{
	return &_eventer;
}
void ViewEngineGL::drawSelection( f2 const &p1 , f2 const &p2 )
{
	selection_box = f4( 0.5f * p1 + f2( 0.5f ) , 0.5f * p2 );
}
f3 ViewEngineGL::getMousePos( f2 const &mpos )
{
	return DrawlerDeffered::getSingleton()->getMousePos( mpos );
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