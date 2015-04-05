#ifndef VIEWENGINEGL_H
#define VIEWENGINEGL_H
#include "export/ViewInterface.h"
#include "WindowGL.h"
#include "../base/REventer.h"
#include "3dgl/RDrawableGL.h"
#include "3dgl/RViewModelsGL.h"
#include <list>
#include "gui/gl/GUIRendererGL.h"
#include "3dgl/DrawlerDeffered.h"
#include <vector>
class Scene3DGL : public Scene3D
{
public:
	RCamera _main_cam;
	std::vector< UnitInstanceState > _instances;
	std::vector< LightState > _lights;
public:
	Scene3DGL();
	ViewTypes::RDrawInstancePTR genInstance() override;
	UnitInstanceState *getInstanceStatePtr( ViewTypes::RDrawInstancePTR i ) override;
	ViewTypes::RLightSourcePTR genLight() override;
	LightState *getLightStatePtr( ViewTypes::RLightSourcePTR i ) override;
	RCamera const *getCamera() const override;
	RCamera *getCamera() override;
	std::vector< UnitInstanceState > const &getStateVector() const override;
	std::vector< LightState > const &getLightVector() const override;
};
class ViewEngineGL : public ViewManager
{
private:
	RWindowGL win;
	RGraphicProgrammGL _quad_prog;
	bool _tess = true;
	RPolyQuadGL _screen_quad;
	REventer _eventer;
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
	REventer *getEventer() override;
	void release() override;
};
#endif // VIEWENGINEGL_H
