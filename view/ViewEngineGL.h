#ifndef VIEWENGINEGL_H
#define VIEWENGINEGL_H
#include "export/ViewInterface.h"
#include <openGL\WindowGL.h>
#include <base/Eventer.h>
#include <openGL\DrawableGL.h>
#include <openGL\ViewModelsGL.h>
#include <list>
#include <openGL\GUIRendererGL.h>
#include "DrawlerDeffered.h"
#include <vector>
class Scene3DGL : public Scene3D
{
public:
	Camera _main_cam;
	std::vector< UnitInstanceState > _instances;
	std::vector< LightState > _lights;
public:
	Scene3DGL();
	uint genInstance() override;
	UnitInstanceState *getInstanceStatePtr( uint i ) override;
	uint genLight() override;
	LightState *getLightStatePtr( uint i ) override;
	Camera const *getCamera() const override;
	Camera *getCamera() override;
	std::vector< UnitInstanceState > const &getStateVector() const override;
	std::vector< LightState > const &getLightVector() const override;
	uint getSelected( f2 const & ) const override;
	std::vector< uint > getSelected( f2 const & , f2 const & ) const override;
};
class ViewEngineGL : public ViewManager
{
private:
	WindowGL win;
	GraphicProgrammGL _quad_prog;
	bool _tess = true;
	PolyQuadGL _screen_quad;
	Eventer _eventer;
	GUIRendererGL _guimng;
	GUILayout const *_gui = nullptr;
	bool _inited = false;
	u2 _resolution;
	Scene3DGL const *_cur_scene = nullptr;
	void tick( int w , int h );
	void updateRes();
public:
	void init() override;
	void setScene( Scene3D const * ) override;
	void setResolution( int , int ) override;
	void setGUI( GUILayout const * ) override;
	Scene3D *genScene() override;
	Scene3D  const *getScene() override;
	Eventer *getEventer() override;
	void drawSelection( f2 const & , f2 const & ) override;
	f3 getMousePos( f2 const & ) override;
	void release() override;
};
#endif // VIEWENGINEGL_H
