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
	std::vector< RDrawableState > _instances;
	std::vector< RLightState > _lights;
public:
	Scene3DGL();
	ViewTypes::RDrawInstancePTR genInstance() override;
	RDrawableState *getInstanceStatePtr(ViewTypes::RDrawInstancePTR i) override;
	ViewTypes::RLightSourcePTR genLight() override;
	RLightState *getLightStatePtr(ViewTypes::RLightSourcePTR i) override;
	RCamera const *getCamera() const override;
	RCamera *getCamera() override;
	std::vector< RDrawableState > const &getStateVector() const override;
	std::vector< RLightState > const &getLightVector() const override;
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
	void tick(int w, int h);
public:
	void init() override;
	void setScene(Scene3D const *scene) override;
	void setGUI(GUILayout const *gui) override;
	Scene3D *genScene() override;
	REventer *getEventer() override;
	void release() override;
};
#endif // VIEWENGINEGL_H
