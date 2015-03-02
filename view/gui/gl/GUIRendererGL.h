#ifndef GIRENDERERGL_H
#define GIRENDERERGL_H
#include "../../Gl.h"
#include "../../../base/RBase.h"
#include "../../3dgl/RTexturesGL.h"
#include "../../../base/RFileloader.h"
#include "../../3dgl/RViewModelsGL.h"
#include "../../3dgl/RDrawableGL.h"
class GUIRendererGL : public RInitable
{
private:
	RTextureHolderGL _font_texture;
	RGraphicProgrammGL _text_shader;
	RPolyQuadGL _screen_quad;
public:
	GUIRendererGL( const GUIRendererGL & ) = delete;
	void operator=( const GUIRendererGL & ) = delete;
	GUIRendererGL() = default;
	void init()
	{
		_font_texture = std::move( RTextureHolderGL( RFileLoader::loadImage( "res/view/images/font1.jpg" ) , 1 ) );
		_font_texture.init();
		_screen_quad.init();
		_text_shader.init( "res/shaders/glsl/text_shader.glsl" , "res/shaders/glsl/screen_quad_vertex.glsl" );
	}
	void drawText( std::string const text , f2 const &pos , float size )
	{
		glEnable( GL_BLEND );
		glBlendFunc( GL_ONE_MINUS_SRC_ALPHA , GL_SRC_ALPHA );
		_text_shader.bind();
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D , _font_texture.__texture_pointer_array[0] );
		glUniform1i( 0 , 0 );
		_screen_quad.draw( InstanceInfo() );
		glDisable( GL_BLEND );
	}
	void release()
	{
		_font_texture.release();
		_screen_quad.release();
		_text_shader.release();
	}
	~GUIRendererGL()
	{
		release();
	}
};
#endif // GIRENDERERGL_H
