#ifndef RSCENEDRAWLER_H
#define RSCENEDRAWLER_H
#include <base/Base.h>
#include <base/Eventer.h>
#include <view\ViewModels.h>
#include <view\Camera.h>
#include <view\GUILayot.h>
//#ifdef _WIN32
/*#if defined( VIEWLIBRARY_EXPORT )
#define VIEWAPI   __declspec( dllexport )
#else
#define VIEWAPI   __declspec( dllimport )
#endif
#else
#define VIEWAPI
#endif*/
#define VIEWAPI
class Scene3D
{
public:
	Scene3D() = default;
	Scene3D( Scene3D const & ) = delete;
	void operator=( Scene3D const & ) = delete;
	virtual uint genInstance() = 0;
	virtual UnitInstanceState *getInstanceStatePtr( uint ) = 0;
	virtual std::vector< UnitInstanceState > const &getStateVector() const = 0;
	virtual std::vector< LightState > const &getLightVector() const = 0;
	virtual std::vector< uint > getSelected( f2 const & , f2 const & ) const = 0;
	virtual uint getSelected( f2 const & ) const = 0;
	virtual uint genLight() = 0;
	virtual LightState *getLightStatePtr( uint ) = 0;
	virtual Camera const *getCamera() const = 0;
	virtual Camera *getCamera() = 0;
};
class ViewManager : public Initable
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
	virtual void setResolution( int , int ) = 0;
	virtual void setScene( Scene3D const * ) = 0;
	virtual void setGUI( GUILayout const * ) = 0;
	virtual void drawSelection( f2 const & , f2 const & ) = 0;
	virtual f3 getMousePos( f2 const & ) = 0;
	virtual Scene3D *genScene() = 0;
	virtual Scene3D const *getScene() = 0;
	virtual Eventer *getEventer() = 0;
};
#endif // RSCENEDRAWLER_H
