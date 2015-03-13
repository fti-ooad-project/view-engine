#ifndef RSCENEDRAWLER_H
#define RSCENEDRAWLER_H
#include "../../base/RBase.h"
#include "../../base/REventer.h"
#include "RViewModels.hpp"
#include "RCamera.hpp"
#ifdef _WIN32
#if defined( VIEWLIBRARY_EXPORT )
#define VIEWAPI   __declspec( dllexport )
#else
#define VIEWAPI   __declspec( dllimport )
#endif
#else
#define VIEWAPI
#endif
class Scene3D
{
public:
	Scene3D() = default;
	Scene3D( Scene3D const & ) = delete;
	void operator=( Scene3D const & ) = delete;
	virtual ViewTypes::RDrawInstancePTR genInstance() = 0;
	virtual RDrawableState *getInstanceStatePtr( ViewTypes::RDrawInstancePTR ) = 0;
	virtual std::vector< RDrawableState > const &getStateVector() const = 0;
	virtual std::vector< RLightState > const &getLightVector() const = 0;
	virtual ViewTypes::RLightSourcePTR genLight() = 0;
	virtual RLightState *getLightStatePtr( ViewTypes::RLightSourcePTR ) = 0;
	virtual RCamera const *getCamera() const = 0;
	virtual RCamera *getCamera() = 0;
};
class GUILayout
{
public:
	GUILayout() = default;
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
	ViewManager() = default;
	ViewManager( ViewManager const & ) = delete;
	void operator=( ViewManager const & ) = delete;
	static VIEWAPI ViewManager *singletonGet( int );
	virtual void init() = 0;
	virtual void setScene( Scene3D const * ) = 0;
	virtual void setGUI( GUILayout const * ) = 0;
	virtual Scene3D *genScene() = 0;
	virtual REventer *getEventer() = 0;
};
#endif // RSCENEDRAWLER_H
