#include <iostream>
#define RLOG
#include "view/ViewEngineGL.hpp"
VIEWAPI ViewManager * ViewManager::singletonGet( int type )
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
	auto scene = engine->genScene();
	auto eventer = engine->getEventer();
	xfor( x , 100 )
		xfor( y , 100 )
		{
			f4x4 temp( 1.0f );
			temp( 3 , 0 ) = 2.0f * ( x - 50.0f );
			temp( 3 , 1 ) = 2.0f * ( - y + 50.0f );
			scene->getInstanceStatePtr( scene->genInstance() )->_view.push_back( ViewInfo{ 0 , temp } );
			//data.push_back( { 0.2 * time , 0.0f , 0 , 0 , 0 , temp } );
		}
	auto cam = scene->getCamera();
	f3 cam_pos , cam_lookat;
	eventer->addKeyFunc(
			[cam]( const KeyStates &cs , const float dt )
			{
				const float dr = dt * 30.0f;
				f3 v( 0.0f , 0.0f , 0.0f );
				if( cs.__cur_states[SDL_SCANCODE_W] )//w
					v += cam->_v3local_z;
				if( cs.__cur_states[SDL_SCANCODE_S] )//s
					v -= cam->_v3local_z;
				if( cs.__cur_states[SDL_SCANCODE_A] )//a
					v -= cam->_v3local_x;
				if( cs.__cur_states[SDL_SCANCODE_D] )//d
					v += cam->_v3local_x;
				float l = v.g_mod();
				if( l != 0.0f )
					cam->pos( cam->_v3pos + v.g_norm() * dr );
				cam->calc();
				//cam->$getViewProj().print();
			}
		);
	eventer->addMouseFunc(
		[cam]( const MouseStates &cs , const float dt )
			{
				static float phi( 1.4f );
				static float theta( 1.4f );
				constexpr float delta = 0.01f;
				const float dr = dt * 50.0f;
				if( cs.__cur_states[0] )
				{
					//MARK
					phi += dr * ( cs.__cur_pos.x() - cs.__last_pos.x() );
					theta += dr * ( cs.__cur_pos.y() - cs.__last_pos.y() );
					if( theta < delta ) theta = delta;
					else if( theta > PI ) theta = PI - delta;
					cam->angle( phi , theta );
					cam->calc();
				}
			}
	);
	//cam->lookAt( cam_pos , cam_lookat , f3( 0.0f , 0.0f , 1.0f ) );
	engine->setScene( scene );
	std::cout << "Hello World!" << std::endl;
	while( true )
	{
		sleep( 0x100 );
	}
}

