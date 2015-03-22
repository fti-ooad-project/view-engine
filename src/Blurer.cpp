#include "../view/3dgl/Blurer.h"
#include "../view/GL.h"
uint Blurer::process( uint buffid , float radius )
{
	_process_prog.bind();
	_process_pass.clear();
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , buffid );
	glUniform1i( 0 , 0 );
	glUniform2f( 1 , 0.0f , 1.0f );
	glUniform1f( 2 , radius );
	glUniform1i( 3 , 5 );
	_screen_quad.draw();
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , _process_pass.getBufferPtr( 0 ) );
	glUniform1i( 0 , 0 );
	glUniform2f( 1 , 1.0f , 0.0f );
	_screen_quad.draw();
	return _process_pass.getBufferPtr( 0 );
}
/*Blurer *Blurer::getSingleton()
{
	static Blurer sng[ 1 ];
	return sng;
}*/
void Blurer::init()
{
	if( isInited() ) return;
	setInited( true );
	_process_prog.init( "res/shaders/glsl/blur.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" );
	_process_pass.init( { { 512 , 512 } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 ,
						false , false , 4 } );
	_screen_quad.init();
}
void Blurer::release()
{
	if( !isInited() ) return;
	setInited( false );
	_process_prog.release();
	_process_pass.release();
	_screen_quad.release();
}