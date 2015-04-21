#ifndef RSCENEDRAWLER_H
#define RSCENEDRAWLER_H
#include <base/Base.h>
#include <base/Eventer.h>
#include <view\ViewModels.h>
#include <view\Camera.h>
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
	virtual uint genLight() = 0;
	virtual LightState *getLightStatePtr( uint ) = 0;
	virtual Camera const *getCamera() const = 0;
	virtual Camera *getCamera() = 0;
};
class GUIElem
{
public:
	class GUIFloat
	{
	public:
		static const int LEFT = 0 , RIGHT = 1 , TOP = 0 , BOTTOM = 1 , CENTER = 2;
	};
	class GUIStatus
	{
	public:
		static const int CLICKED = 0 , NONE = 1 , HOVER = 2 , DRAG = 3;
	};
	std::string _text;
	f2 _size_pix;
	mutable f2 _calculated_pos;
	mutable f2 _calculated_size;
	int _floatX , _floatY , _layer;
	int _status;
	bool _visible;
	std::function< void() > _onClick;
	std::function< void( f2 const & ) > _onDrag;
	std::vector< std::shared_ptr< GUIElem > > _child;
};
class GUILayout
{
private:
	std::vector< std::shared_ptr< GUIElem > > _elems;
public:
	GUILayout() = default;
	GUILayout( GUILayout const & ) = delete;
	GUILayout operator=( GUILayout const & ) = delete;
	void addElem( std::shared_ptr< GUIElem > elem )
	{
		_elems.push_back( elem );
	}
	std::vector< std::shared_ptr< GUIElem > > const getElemVec() const
	{
		return _elems;
	}
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
	virtual Scene3D *genScene() = 0;
	virtual Scene3D const *getScene() = 0;
	virtual Eventer *getEventer() = 0;
};
#endif // RSCENEDRAWLER_H
