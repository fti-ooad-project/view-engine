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
	xfor( x , 30 )
		xfor( y , 30 )
		{
			f4x4 temp( 1.0f );
			temp( 3 , 0 ) = 2.0f * ( x - 15.0f );
			temp( 3 , 1 ) = 2.0f * ( -y + 15.0f );
			//temp( 3 , 2 ) = -2.0f;
			scene->getInstanceStatePtr( scene->genInstance() )->_view.push_back( ViewInfo{ 0 , temp } );
		}
	f4x4 temp( 1.0f );
	scene->getInstanceStatePtr( scene->genInstance() )->_view.push_back( ViewInfo{ 2 , temp } );

	RLightState *ls = scene->getLightStatePtr( scene->genLight() );
	ls->_cast_shadow = true;
	ls->_colori = f4( 1.0f , 1.0f , 1.0f , 1.0f );
	ls->_dir = f3( -0.7f , 0.0f , -0.7f );
	ls->_pos = f3( 70.0f , 0.0f , 70.0f );
	ls->_type = RLightSourceType::RLIGHT_DIRECT;
	auto cam = scene->getCamera();
	cam->lookAt( f3( 0.0f , -2.0f , 2.0f ) , f3( 0.0f , 0.0f , 0.0f ) , f3( 0.0f , 0.0f , 1.0f ) );
	f3 cam_pos , cam_lookat;
	eventer->addKeyFunc(
			[cam,ls]( const KeyStates &cs , const float dt )
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
				f2 clook = f2( cam->_v3local_z.x() , cam->_v3local_z.y() ).g_norm() * 10.0f;

				ls->_pos = f3( 70.0f , 0.0f , 30.0f );
				ls->_dir = -ls->_pos.g_norm();
				ls->_pos += f3( cam->_v3pos.x() + cam->_v3local_z.x() * 20.0f , cam->_v3pos.y() + cam->_v3local_z.y() * 20.0f , 0.0f );
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
	//std::cout << "Hello World!" << std::endl;
	float t = 0.0f;
	while( true )
	{
		t += 1.0f / 0x20;
		sleep( 0x20 );
		/*xfor( x , 3 )
		xfor( y , 3 )
		{
			scene->getInstanceStatePtr( x + y * 3 )->_view[0].model( 3 , 0 ) += 0.1f * sinf( 4.0f * t );
			scene->getInstanceStatePtr( x + y * 3 )->_view[0].model( 3 , 1 ) += 0.1f * cosf( 4.0f * t );
			scene->getInstanceStatePtr( x + y * 3 )->_view[0].model( 3 , 2 ) = -1.0f;
		}*/
		//ls->_pos = f3( sinf( t ) , cosf( t ) , 1.0f ) * 50.0f;
		//ls->_dir = -ls->_pos.g_norm();
		//ls->_dir.print();
	}
}

