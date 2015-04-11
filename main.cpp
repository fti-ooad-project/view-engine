#include <iostream>
#define RLOG
#include <memory>
#include "view/ViewEngineGL.h"
#include <stdexcept>
#include "view/GL.h"
void assertGL( int mark )
{
	if( int i = glGetError() )
	{
#ifdef RLOG
		std::cout << "ogl err at " << mark << ":" << i << "\n";
#endif
		throw std::logic_error( "opengl assert error" );
	}
}
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
	xfor( x , 10 )
		xfor( y , 10 )
	{
		auto *state = scene->getInstanceStatePtr( scene->genInstance() );
		state->_view.push_back( 0 );
		state->_animstat.update( float( x + y ) / 5.0f );
		state->_animstat._speed = 0.4f;
		state->_pos = f3( x , y , 0.0f );
		state->_up = f3( 0.0f , 0.0f , 1.0f );
		state->_look = f3( 1.0f , 0.0f , 0.0f );
		state->_auto_height = true;
	}
	{
		auto *state = scene->getInstanceStatePtr( scene->genInstance() );
		state->_view.push_back( 2 );
		state->_pos = f3( 0.0f , 0.0f , 0.0f );
		state->_up = f3( 0.0f , 0.0f , 1.0f );
		state->_look = f3( 1.0f , 0.0f , 0.0f );
		state->_auto_height = true;
	}
	/*{
		f4x4 temp( 1.0f );
		temp( 0 , 0 ) = 0.0f;
		temp( 1 , 0 ) = 1.0f;
		temp( 0 , 1 ) = 1.0f;
		temp( 1 , 1 ) = 0.0f;
		temp( 3 , 2 ) = 20.0f;
		temp( 3 , 0 ) = 0.0f;
		auto *state = scene->getInstanceStatePtr( scene->genInstance() );
		state->_view.push_back( 3 );
		state->model = temp;
	}*/
	LightState *ls = scene->getLightStatePtr( scene->genLight() );
	ls->_cast_shadow = true;
	ls->_colori = f4( 0.9f , 0.99f , 1.0f , 5.0f );
	ls->_dir = f3( -0.7f , 0.0f , -0.7f );
	ls->_pos = f3( 70.0f , 0.0f , 70.0f );
	ls->_type = RLightSourceType::RLIGHT_DIRECT;

	LightState *ss = scene->getLightStatePtr( scene->genLight() );
	ss->_cast_shadow = false;
	ss->_colori = f4( 0.9f , 0.3f , 0.4f , 500.0f );
	ss->_pos = f3( -10.0f , 0.0f , 5.0f );
	ss->_type = RLightSourceType::RLIGHT_OMNI;
	ss->_size = 1.0f;

	auto cam = scene->getCamera();
	cam->lookAt( f3( 0.0f , -2.0f , 2.0f ) , f3( 0.0f , 0.0f , 0.0f ) , f3( 0.0f , 0.0f , 1.0f ) );
	f3 cam_pos( 0.0f ) , cam_lookat;
	float X = 0.0f;
	float cam_z = 10.0f;
	ls = scene->getLightStatePtr( 0 );
	eventer->addTimeFunc(
		[ ss ]( const float dt )
	{
		static float t = 0.0f;
		t += dt;
		ss->_colori = f4( 0.9f , 0.8f , 0.4f , 500.0f + 100.0f * sinf( t * 30.0f ) );
	}
		);
	eventer->addKeyFunc(
		[ cam , ls , ss , &X , &cam_pos , &cam_z ]( const KeyStates &cs , const float dt )
	{
		const float dr = dt * 30.0f;
		f3 v( 0.0f , 0.0f , 0.0f );
		
		/*if( cs.__cur_states[SDL_SCANCODE_X] )
			_tri = true;
		else
			_tri = false;*/
		if( cs.__cur_states[ SDL_SCANCODE_W ] )//w
			v += //f3( 1.0f , 1.0f , 0.0f );// 
		cam->_v3local_z;
		if( cs.__cur_states[ SDL_SCANCODE_S ] )//s
			v -= //f3( 1.0f , 1.0f , 0.0f );//
		cam->_v3local_z;
		if( cs.__cur_states[ SDL_SCANCODE_A ] )//a
			v -= //f3( 1.0f , -1.0f , 0.0f );//
		cam->_v3local_x;
		if( cs.__cur_states[ SDL_SCANCODE_D ] )//d
			v += //f3( 1.0f , -1.0f , 0.0f );//
		cam->_v3local_x;
		if( cs.__cur_states[ SDL_SCANCODE_X ] )//d
			X += dr * 0.3f;
		if( cs.__cur_states[ SDL_SCANCODE_Z ] )//d
			X -= dr * 0.3f;
		float l = v.g_mod();
		if( l != 0.0f )
			cam->_v3pos += v.g_norm() * dr;
			//cam_pos += v.g_norm() * dr;
			//cam->lookAt( cam_pos + cam_z * f3( -1.0f , -1.0f , 1.0f ) , cam_pos - 100.0f * f3( -1.0f , -1.0f , 1.0f ) , f3( 0.0f , 0.0f , 1.0f ) );
		cam->calc();
		f2 clook = f2( cam->_v3local_z.x() , cam->_v3local_z.y() ).g_norm() * 10.0f;

		ls->_pos = f3( 70.0f , 0.0f , 70.0f );
		ls->_dir = -ls->_pos.g_norm();
		ls->_pos += f3( cam->_v3pos.x() , cam->_v3pos.y() , 0.0f );
		//cam->$getViewProj().print();
	}
	);
	std::shared_ptr< GUILayout > main_menu( new GUILayout() );
	std::shared_ptr< GUIElem > button1( new GUIElem() );
	button1->_floatX = GUIElem::GUIFloat::LEFT;
	button1->_floatY = GUIElem::GUIFloat::TOP;
	button1->_layer = 0;
	button1->_text = "menu";
	button1->_size_pix = f2( 40.0f , 30.0f );
	button1->_visible = true;
	button1->_onClick = [ engine , scene ]()
	{
		if( engine->getScene() )
			engine->setScene( nullptr );
		else
			engine->setScene( scene );
	};
	main_menu->addElem( button1 );
	std::shared_ptr< GUIElem > button2( new GUIElem() );
	button2->_floatX = GUIElem::GUIFloat::LEFT;
	button2->_floatY = GUIElem::GUIFloat::TOP;
	button2->_layer = 0;
	button2->_text = "but2";
	button2->_size_pix = f2( 40.0f , 30.0f );
	button2->_visible = true;
	button2->_onClick = nullptr;
	main_menu->addElem( button2 );

	eventer->addMouseFunc(
		[ cam , &cam_z , main_menu ]( const MouseStates &cs , const float dt )
	{
		static float phi( 1.4f );
		static float theta( 1.4f );
		constexpr float delta = 0.01f;
		const float dr = dt * 50.0f;
		cam_z += ( cs._mwheel - cs._mwheel_last ) * 1.0f;
		if( cs.__cur_states[ 0 ] )
		{
			
			//MARK
			phi += dr * ( cs.__cur_pos.x() - cs.__last_pos.x() );
			theta += dr * ( cs.__cur_pos.y() - cs.__last_pos.y() );
			//LOG << phi << ;
			if( theta < delta ) theta = delta;
			else if( theta > PI ) theta = PI - delta;
			cam->angle( phi , theta );
			cam->calc();
		}
		auto isin = []( f2 const &p , f2 const &c , f2 const &s )
		{
			if(
				fabsf( -p.x() - c.x() ) < s.x()
				&& fabsf( -p.y() - c.y() ) < s.y()
				)
				return true;
			return false;
		};
		for( auto &i : main_menu->getElemVec() )
		{
			if( isin( cs.__cur_pos , i->_calculated_pos , i->_calculated_size ) )
			{
				if( cs.__cur_states[ 0 ] )
					i->_status = 2;
				else
					i->_status = 1;
				if( !cs.__cur_states[ 0 ] && cs.__last_states[ 0 ] )
					if( i->_onClick )
						i->_onClick();
			}
			else
				i->_status = 0;
			//LOG << "clicked\n";
		}
	}
	);
	//cam->lookAt( cam_pos , cam_lookat , f3( 0.0f , 0.0f , 1.0f ) );
	
	
	engine->setGUI( main_menu.get() );
	engine->setScene( scene );
	//std::cout << "Hello World!" << std::endl;
	float t = 0.0f;
	while( true )
	{
		t += 1.0f / 0x20;
		sleep( 0x10 );
		xfor( x , 10 )
		xfor( y , 10 )
		{
			auto *state = scene->getInstanceStatePtr( x + y * 10 );
			state->_pos = f3( x + X , y , 0.0f );
		}
		//ls->_pos = f3( sinf( t ) , cosf( t ) , 1.0f ) * 50.0f;
		//ls->_dir = -ls->_pos.g_norm();
		//ls->_dir.print();
	}
	return 0;
}

