#include "../view/3dgl/HeighMapDrawler.h"
#include "../GlslDefines.h"
void HeightMapDrawler::bindToDraw()
{
	_prog.bind();
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D , _hmap.getTexture() );
	glUniform1i( 3 , 0 );
	glActiveTexture( GL_TEXTURE0 + 1 );
	glBindTexture( GL_TEXTURE_2D_ARRAY , _textures.getTexture() );
	glUniform1i( 1 , 1 );
	glUniform1i( FLAGS , ShaderMask::MASK_TEXTURED | ShaderMask::MASK_TEXTURED_DIF );
	glUniform4f( 4 , _pos.x() , _pos.y() , _size.x() , _size.y() );
}
void HeightMapDrawler::draw( bool tess )
{
	glBindVertexArray( _vao );
	if( tess )
		glDrawElements( GL_PATCHES , _indx_count , GL_UNSIGNED_SHORT , 0 );
	else
		glDrawElements( GL_TRIANGLES , _indx_count , GL_UNSIGNED_SHORT , 0 );
}
void HeightMapDrawler::init( uint density , f2 const &size , f2 const &bpos )
{
	if( isInited() ) return;
	setInited( true );
	_size = size;
	_pos = bpos;
	_hmap = std::move( RTextureHolderGL( std::move( RFileLoader::loadImage( "res/view/images/terrain.png" ) ) , 1 ) );
	_hmap.init();
	std::string difftexname[] = { "res/view/images/dirtnorm.jpg" , "res/view/images/grass.jpg" , "res/view/images/snowdirt.jpg" , "res/view/images/ground.jpg" };
	_textures = std::move( RTextureHolderGL( std::move( RFileLoader::loadImage( difftexname , 4 ) ) , 4 ) );
	_textures.init();
	std::unique_ptr< f3[] > pos( new f3[ density * density ] );
	_indx_count = ( density - 1 ) * ( density - 1 ) * 6;
	std::unique_ptr< unsigned short[] > index( new unsigned short[ _indx_count ] );
	xfor( x , density )
		xfor( y , density )
	{
		pos[ x + y * density ] = f3( -1.0f + 2.0f * float( x ) / ( density - 1 ) , -1.0f + 2.0f * float( y ) / ( density - 1 ) , 0.0f );
	}
	xfor( x , density - 1 )
		xfor( y , density - 1 )
	{
		uint id = x * 6 + y * ( density - 1 ) * 6;
		index[ id ] = x + y * density;
		index[ id + 1 ] = ( x + 1 ) + y * density;
		index[ id + 2 ] = ( x + 1 ) + ( y + 1 ) * density;
		index[ id + 3 ] = x + y * density;
		index[ id + 4 ] = ( x + 1 ) + ( y + 1 ) * density;
		index[ id + 5 ] = x + ( y + 1 ) * density;
	}
	glGenVertexArrays( 1 , &_vao );
	glBindVertexArray( _vao );
	uint vbo , ibo;
	glGenBuffers( 1 , &vbo );
	glBindBuffer( GL_ARRAY_BUFFER_ARB , vbo );
	glBufferData( GL_ARRAY_BUFFER_ARB , density * density * sizeof( f3 ) , pos.get() , GL_STATIC_DRAW_ARB );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , sizeof( f3 ) , reinterpret_cast< void* >( 0 ) );
	glGenBuffers( 1 , &ibo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB , ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER_ARB , _indx_count * sizeof( unsigned short ) , index.get() , GL_STATIC_DRAW_ARB );
	glBindVertexArray( 0 );
	glDeleteBuffers( 1 , &vbo );
	glDeleteBuffers( 1 , &ibo );
	_prog.init( "res/shaders/glsl/height_map_frag.glsl" , "res/shaders/glsl/height_map_vertex_tess.glsl" , "res/shaders/glsl/height_map_geom.glsl" , "res/shaders/glsl/height_map_tess_tc.glsl" , "res/shaders/glsl/height_map_tess_te.glsl" );
	LOG << "ef\n";
}
void HeightMapDrawler::release()
{
	if( !isInited() ) return;
	setInited( false );
	glDeleteVertexArrays( 1 , &_vao );
	_prog.release();
	_lightprog.release();
	_textures.release();
	_hmap.release();
}
HeightMapDrawler *HeightMapDrawler::getSingleton()
{
	static HeightMapDrawler sng[ 1 ];
	return sng;
}