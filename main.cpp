#include <iostream>
#define RLOG
#include "view/ViewEngineGL.hpp"
ViewManager *ViewManager::singletonGet( int type )
{
	static ViewManager* singleton = nullptr;
	if( singleton ) return singleton;
	switch( type )
	{
		case ViewManager::API::GL:
			return singleton = new ViewEngineGL;
		break;
	}
	return nullptr;
}
#undef main
int main()
{
	ViewManager *engine = ViewManager::singletonGet( ViewManager::API::GL );
	engine->init();
	std::cout << "Hello World!" << std::endl;
	while( true )
	{
		sleep( 0x10 );
	}
	return 0;
}

