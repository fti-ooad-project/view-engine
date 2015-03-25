#ifndef RSCENEDRAWLER_H
#define RSCENEDRAWLER_H
#include "../../base/RBase.h"
#include "../../base/REventer.h"
#include "RViewModels.h"
#include "RCamera.h"
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
	virtual ViewTypes::RDrawInstancePTR genInstance() = 0;
	virtual RDrawableState *getInstanceStatePtr( ViewTypes::RDrawInstancePTR ) = 0;
	virtual std::vector< RDrawableState > const &getStateVector() const = 0;
	virtual std::vector< RLightState > const &getLightVector() const = 0;
	virtual ViewTypes::RLightSourcePTR genLight() = 0;
	virtual RLightState *getLightStatePtr( ViewTypes::RLightSourcePTR ) = 0;
	virtual RCamera const *getCamera() const = 0;
	virtual RCamera *getCamera() = 0;
};
class GUIElem
{
public:
	static class GUIFloat
	{
	public:
		static const int LEFT = 0 , RIGHT = 1 , TOP = 0 , BOTTOM = 1 , CENTER = 2;
	};
	std::string _text;
	f2 _size_pix;
	//f2 _pos_pix;
	int _floatX , _floatY , _layer;
	int _type;
	bool _visible;
	std::function< void() > _onClick;
	std::function< void( f2 const & ) > _onDrag;
	std::vector< std::shared_ptr< GUIElem > > _child;
};
class GUILayout
{
private:
	std::vector< std::shared_ptr< GUIElem > > _elem_vec;
public:
	GUILayout() = default;
	GUILayout( GUILayout const & ) = delete;
	GUILayout operator=( GUILayout const & ) = delete;
	void addElem( std::shared_ptr< GUIElem > elem )
	{
		_elem_vec.push_back( elem );
	}
	std::vector< std::shared_ptr< GUIElem > > const getElemVec() const
	{
		return _elem_vec;
	}
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
	virtual void setResolution( int , int ) = 0;
	virtual void setScene( Scene3D const * ) = 0;
	virtual void setGUI( GUILayout const * ) = 0;
	virtual Scene3D *genScene() = 0;
	virtual REventer *getEventer() = 0;
};
#endif // RSCENEDRAWLER_H
