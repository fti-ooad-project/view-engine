#ifndef RSCENEDRAWLER_H
#define RSCENEDRAWLER_H
#include "base/RBase.h"
#include "base/REventer.h"
#include "view/RViewModels.h"
#include "view/RDrawable.h"
#include "view/RLight.h"
class ViewManager : public RInitable
{
public:
	class API
	{
	public:
		static const int GL = 0;
	};
	ViewManager( ViewManager const & ) = delete;
	ViewManager operator=( ViewManager const & ) = delete;
	static ViewManager *singletonGet( int );
	virtual void init() = 0;
	virtual void drawScene( Scene3D const * ) = 0;
	virtual void drawGUI( GUILayout const * ) = 0;
	class GUILayout
	{
	public:
		GUILayout( GUILayout const & ) = delete;
		GUILayout operator=( GUILayout const & ) = delete;
	};
	class Scene3D
	{
	public:
		Scene3D( Scene3D const & ) = delete;
		Scene3D operator=( Scene3D const & ) = delete;
		///Instances
		ViewTypes::RDrawInstancePTR genInstance();
		RDrawableState *getInstanceStatePtr( ViewTypes::RDrawInstancePTR );
		///Lights
		ViewTypes::RLightSourcePTR genLight();
		RLightState *getLightStatePtr( ViewTypes::RLightSourcePTR );
		RCamera *getCamera();
		REventer *getEventer();
	};
};
#endif // RSCENEDRAWLER_H
