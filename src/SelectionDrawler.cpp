#include "../view/3dgl/SelectionDrawler.h"
#include "../view/3dgl/Blurer.h"
#include "../view/GL.h"
SelectionDrawler::SelectionDrawler()
{
}
uint SelectionDrawler::process( uint buffid )
{
	_process_prog.bind();
	_process_pass.bind();
	_process_pass.clear();
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , buffid );
	glUniform1i( 0 , 0 );
	glUniform1i( 2 , 0 );
	_screen_quad.draw();
	uint blured = _blurer.process( _process_pass.getBufferPtr( 0 ) , 0.02f );
	_process_prog.bind();
	_process_pass.bind();
	_process_pass.clear();
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , buffid );
	glUniform1i( 0 , 0 );
	glUniform1i( 2 , 1 );
	glActiveTexture( GL_TEXTURE0 + 1 );
	glBindTexture( GL_TEXTURE_2D , blured );
	glUniform1i( 1 , 1 );
	_screen_quad.draw();
	return _process_pass.getBufferPtr( 0 );
}
SelectionDrawler * SelectionDrawler::getSingleton()
{
	static SelectionDrawler sng[ 1 ];
	return sng;
}
void SelectionDrawler::init()
{
	if( isInited() ) return;
	setInited( true );
	_process_prog.init( "res/shaders/glsl/selectableprocessfrag.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" );
	_process_pass.init( { { 512 , 512 } , RBufferStoreType::RBUFFER_FLOAT , 1 , -1 ,
						false , false , 4 } );
	_screen_quad.init();
	_blurer.init();
}
void SelectionDrawler::release()
{
	if( !isInited() ) return;
	setInited( false );
	_screen_quad.release();
	_process_pass.release();
	_process_prog.release();
	_blurer.release();
}
