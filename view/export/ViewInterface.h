#pragma once
#ifndef RSCENEDRAWLER_H
#define RSCENEDRAWLER_H
#include <base/Base.h>
#include <base/Eventer.h>
#include <view/ViewModels.h>
#include <view/Camera.h>
#include <view/GUILayot.h>
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
struct GraphicSettings
{
	bool _reflect_water = false;
	bool _draw_shadows = true;
	bool _draw_fog = false;
	bool _tesselation = false;
	uint _screen_width = 1024 , _screen_height = 1024;
	bool operator!=( GraphicSettings const &set )
	{
		if( this->_reflect_water != set._reflect_water )
			return true;
		if( this->_draw_fog != set._draw_fog )
			return true;
		if( this->_draw_shadows != set._draw_shadows )
			return true;
		if( this->_screen_height != set._screen_height )
			return true;
		if( this->_screen_width != set._screen_width )
			return true;
		if( this->_tesselation != set._tesselation )
			return true;
		return false;
	}
};
class ViewManager : public Initable
{
private:
	static GraphicSettings _graph_settings;
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
	static GraphicSettings getGraphicSettings()
	{
		return _graph_settings;
	}
	static void setGraphicSettings( GraphicSettings const &gset )
	{
		_graph_settings = gset;
	}
	virtual void init() = 0;
	virtual void setScene( Scene3D const * ) = 0;
	virtual void setGUI( GUILayout const * ) = 0;
	//virtual GUILayout const *getGUI() const = 0;
	virtual void drawSelection( f2 const & , f2 const & ) = 0;
	virtual f3 getMousePos( f2 const & ) = 0;
	virtual Scene3D *genScene() = 0;
	virtual Scene3D const *getScene() = 0;
	virtual Eventer *getEventer() = 0;
};
#endif // RSCENEDRAWLER_H
