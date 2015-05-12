#include "FogDrawler.h"
#include <openGL\GLincludes.h>
#include <openGL\DrawableGL.h>
FogDrawler * FogDrawler::getSIngleton()
{
	static FogDrawler *sngl = new FogDrawler();
	return sngl;
}
void FogDrawler::init( uint w , uint h )
{
	if( isInited() )
		return;
	setInited( true );
	ScreenQuadGL::getSingleton()->init();
	_pass.init( { { w , h } , BufferStoreType::BUFFER_FLOAT , 1 , -1 , false , false , 3 } );
	_prog.init( "res/shaders/glsl/fogpass_fragment.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" );
}
void FogDrawler::release()
{
	if( !isInited() )
		return;
	setInited( false );
	_pass.release();
	_prog.release();
}
DrawPassGL const &FogDrawler::process( uint lcount , uint depthbuf , DrawPassGL const *lightpasses , f4x4 const *lightviewproj , Camera const *cam )
{
	_prog.bind();
	glUniform1i( 0 , lcount );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , depthbuf );
	glUniform1i( 1 , 0 );
	ito( lcount )
	{
		lightpasses[ i ].bindDepth( 2 + i , 1 + i );
		glUniformMatrix4fv( 20 + i , 1 , false , lightviewproj[ i ].getPtr() );
	}
	glUniform3fv( 12 , 1 , cam->_v3pos.getArray() );
	glUniform3fv( 12 + 1 , 1 , cam->_v3local_z.getArray() );
	f3 cx = cam->_v3local_x * tanf( cam->_fovx / 2.0f );
	f3 cy = cam->_v3local_y * tanf( cam->_fovy / 2.0f );
	glUniform3fv( 12 + 2 , 1 , cx.getArray() );
	glUniform3fv( 12 + 3 , 1 , cy.getArray() );
	_pass.clear();
	_pass.bind();
	ScreenQuadGL::getSingleton()->draw();
	return _pass;
}
