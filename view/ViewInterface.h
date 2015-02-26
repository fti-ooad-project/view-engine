#ifndef RSCENEDRAWLER_H
#define RSCENEDRAWLER_H
#include "base/RBase.h"
#include "base/REventer.h"
#include "3dgl/RViewModels.h"
#include "3dgl/RDrawable.h"
#include "3dgl/RLight.h"
class Scene3D
{
public:
	Scene3D(){}
	Scene3D( Scene3D const & ) = delete;
	void operator=( Scene3D const & ) = delete;
	virtual ViewTypes::RDrawInstancePTR genInstance() = 0;
	virtual RDrawableState *getInstanceStatePtr( ViewTypes::RDrawInstancePTR ) = 0;
	virtual ViewTypes::RLightSourcePTR genLight() = 0;
	virtual RLightState *getLightStatePtr( ViewTypes::RLightSourcePTR ) = 0;
	virtual RCamera *getCamera() = 0;
	virtual REventer *getEventer() = 0;
};
class GUILayout
{
public:
	GUILayout( GUILayout const & ) = delete;
	GUILayout operator=( GUILayout const & ) = delete;
};
class ViewManager : public RInitable
{
public:
	class API
	{
	public:
		static const int GL = 0;
	};
	ViewManager(){}
	ViewManager( ViewManager const & ) = delete;
	void operator=( ViewManager const & ) = delete;
	static ViewManager *singletonGet( int );
	virtual void init() = 0;
	virtual void drawScene( Scene3D const * ) = 0;
	virtual void drawGUI( GUILayout const * ) = 0;
	virtual Scene3D *genScene() = 0;
};
#endif // RSCENEDRAWLER_H
