#include "../view/ViewEngineGL.h"
Scene3DGL::Scene3DGL()
{
}
ViewTypes::RDrawInstancePTR Scene3DGL::genInstance()
{
	_instances.push_back(RDrawableState());
	return _instances.size() - 1;
}
RDrawableState *Scene3DGL::getInstanceStatePtr(ViewTypes::RDrawInstancePTR i)
{
	return &_instances[i];
}
ViewTypes::RLightSourcePTR Scene3DGL::genLight()
{
	_lights.push_back(RLightState());
	return _lights.size() - 1;
}
RLightState *Scene3DGL::getLightStatePtr(ViewTypes::RLightSourcePTR i)
{
	return &_lights[i];
}
RCamera const *Scene3DGL::getCamera() const
{
	return &_main_cam;
}
RCamera *Scene3DGL::getCamera()
{
	return &_main_cam;
}
std::vector< RDrawableState > const &Scene3DGL::getStateVector() const
{
	return _instances;
}
std::vector< RLightState > const &Scene3DGL::getLightVector() const
{
	return _lights;
}
void ViewEngineGL::tick(int w, int h)
{
	if (!_inited)
	{
		DrawlerDeffered::getSingleton()->init();
		_screen_quad.init();
		_inited = true;
		_quad_prog.init("res/shaders/glsl/screen_quad_frag.glsl", "res/shaders/glsl/screen_quad_vertex.glsl");
		//_guimng.init();
		//_guimng.genText( "start game" );
	}
	uint draw = DrawlerDeffered::getSingleton()->draw(_cur_scene, w, h);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.5f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	_quad_prog.bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, draw);
	glUniform1i(0, 0);
	_screen_quad.draw();
	//_guimng.drawPanel( f2( 0.0f , 0.0f ) , f2( 0.2f , 0.2f ) + 0.1f * f2( sin( _cur_time ) , cos( _cur_time ) ) , 0 );
	//_guimng.drawText( 0 , f2( 0.0f , 0.0f ) , f2( 0.2f , 0.1f ) );
}
void ViewEngineGL::init()
{
	if (isInited()) return;
	setInited(true);
	win.init([this](int w, int h) { tick(w, h); }, [this]() { this->release(); }, &_eventer);
}
void ViewEngineGL::setScene(Scene3D const *scene)
{
	_cur_scene = dynamic_cast<Scene3DGL const *>(scene);
}
void ViewEngineGL::setGUI(GUILayout const *gui)
{
}
Scene3D *ViewEngineGL::genScene()
{
	return new Scene3DGL();
}
REventer *ViewEngineGL::getEventer()
{
	return &_eventer;
}
void ViewEngineGL::release()
{
	if (!isInited()) return;
	setInited(false);
	DrawlerDeffered::getSingleton()->release();
	_quad_prog.release();
	_screen_quad.release();

	//WaterSimulator::getSingleton()->release();
}