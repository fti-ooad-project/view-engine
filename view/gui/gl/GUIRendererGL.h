#ifndef GIRENDERERGL_H
#define GIRENDERERGL_H
#include "../../GL.h"
#include "../../../base/RBase.h"
#include "../../3dgl/RTexturesGL.h"
#include "../../../base/RFileloader.h"
#include "../../3dgl/RViewModelsGL.h"
#include "../../3dgl/RDrawableGL.h"
#include "SDL2/SDL_ttf.h"
class PanelDrawler : public RInitable
{
private:
	RTextureHolderGL _field_tex;
	uint _vao[9];
public:
	void init()
	{
		if( isInited() ) return;
		setInited( true );
		uint vbo , ibo;
		static constexpr float quad[9][20] =
		{
			{ -1.0f , -1.0f , 0.0f , 1.0f , 1.0f ,
			1.0f , -1.0f , 0.0f , 1.0f , 1.0f ,
			1.0f , 1.0f , 0.0f , 1.0f , 1.0f ,
			-1.0f , 1.0f , 0.0f , 1.0f , 1.0f } ,
			{ -1.0f , -1.0f , 0.0f , 0.0f , 0.0f ,
			1.0f , -1.0f , 0.0f , 1.0f , 0.0f ,
			1.0f , 1.0f , 0.0f , 1.0f , 1.0f ,
			-1.0f , 1.0f , 0.0f , 0.0f , 1.0f } ,
			{ -1.0f , -1.0f , 0.0f , 1.0f , 0.0f ,
			1.0f , -1.0f , 0.0f , 1.0f , 0.0f ,
			1.0f , 1.0f , 0.0f , 1.0f , 1.0f ,
			-1.0f , 1.0f , 0.0f , 1.0f , 1.0f } ,
			{ -1.0f , -1.0f , 0.0f , 1.0f , 0.0f ,
			1.0f , -1.0f , 0.0f , 0.0f , 0.0f ,
			1.0f , 1.0f , 0.0f , 0.0f , 1.0f ,
			-1.0f , 1.0f , 0.0f , 1.0f , 1.0f } ,
			{ -1.0f , -1.0f , 0.0f , 1.0f , 1.0f ,
			1.0f , -1.0f , 0.0f , 0.0f , 1.0f ,
			1.0f , 1.0f , 0.0f , 0.0f , 1.0f ,
			-1.0f , 1.0f , 0.0f , 1.0f , 1.0f } ,
			{ -1.0f , -1.0f , 0.0f , 1.0f , 1.0f ,
			1.0f , -1.0f , 0.0f , 0.0f , 1.0f ,
			1.0f , 1.0f , 0.0f , 0.0f , 0.0f ,
			-1.0f , 1.0f , 0.0f , 1.0f , 0.0f } ,
			{ -1.0f , -1.0f , 0.0f , 1.0f , 1.0f ,
			1.0f , -1.0f , 0.0f , 1.0f , 1.0f ,
			1.0f , 1.0f , 0.0f , 1.0f , 0.0f ,
			-1.0f , 1.0f , 0.0f , 1.0f , 0.0f } ,
			{ -1.0f , -1.0f , 0.0f , 0.0f , 1.0f ,
			1.0f , -1.0f , 0.0f , 1.0f , 1.0f ,
			1.0f , 1.0f , 0.0f , 1.0f , 0.0f ,
			-1.0f , 1.0f , 0.0f , 0.0f , 0.0f } ,
			{ -1.0f , -1.0f , 0.0f , 0.0f , 1.0f ,
			1.0f , -1.0f , 0.0f , 1.0f , 1.0f ,
			1.0f , 1.0f , 0.0f , 1.0f , 1.0f ,
			-1.0f , 1.0f , 0.0f , 0.0f , 1.0f }
		};
		static constexpr ushort indx[] =
		{
			0 , 1 , 2 , 0 , 2 , 3
		};
		glGenVertexArrays( 9 , _vao );
		ito( 9 )
		{
			glBindVertexArray( _vao[i] );
			glGenBuffers( 1 , &vbo );
			glBindBuffer( GL_ARRAY_BUFFER_ARB , vbo );
			glBufferData( GL_ARRAY_BUFFER_ARB , 80 , quad[i] , GL_STATIC_DRAW_ARB );
			glEnableVertexAttribArray( 0 );
			glEnableVertexAttribArray( 1 );
			glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , 20 , reinterpret_cast< void* >( 0 ) );
			glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , 20 , reinterpret_cast< void* >( 12 ) );
			glGenBuffers( 1 , &ibo );
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB , ibo );
			glBufferData( GL_ELEMENT_ARRAY_BUFFER_ARB , 12 , indx , GL_STATIC_DRAW_ARB );
			glBindVertexArray( 0 );
			glDeleteBuffers( 1 , &vbo );
			glDeleteBuffers( 1 , &ibo );
			glBindBuffer( GL_ARRAY_BUFFER_ARB , 0 );
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB , 0 );
		}
		_field_tex.init( std::move( RFileLoader::loadImage( "res/view/images/gui_field.png" ) ) , 1 );
		_field_tex.setRepeat( false );
	}
	void drawPanel( f2 const &pos , f2 const &size , float border )
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D , _field_tex.getTexture() );
		glUniform1i( 0 , 0 );

		f2 npos = pos;
		f2 nsize = size - f2( border , border );
		glUniform4f( 5 , npos.x() , npos.y() , nsize.x() , nsize.y() );
		glBindVertexArray( _vao[0] );
		glDrawElements( GL_TRIANGLES , 6 , GL_UNSIGNED_SHORT , 0 );

		npos = pos + f2( -size.x() , -size.y() );
		nsize = f2( border , border );
		glUniform4f( 5 , npos.x() , npos.y() , nsize.x() , nsize.y() );
		glBindVertexArray( _vao[1] );
		glDrawElements( GL_TRIANGLES , 6 , GL_UNSIGNED_SHORT , 0 );

		npos = pos + f2( 0.0f , -size.y() );
		nsize = f2( size.x() - border , border );
		glUniform4f( 5 , npos.x() , npos.y() , nsize.x() , nsize.y() );
		glBindVertexArray( _vao[2] );
		glDrawElements( GL_TRIANGLES , 6 , GL_UNSIGNED_SHORT , 0 );

		npos = pos + f2( size.x() , -size.y() );
		nsize = f2( border , border );
		glUniform4f( 5 , npos.x() , npos.y() , nsize.x() , nsize.y() );
		glBindVertexArray( _vao[3] );
		glDrawElements( GL_TRIANGLES , 6 , GL_UNSIGNED_SHORT , 0 );

		npos = pos + f2( size.x() , 0.0f );
		nsize = f2( border , size.y() - border );
		glUniform4f( 5 , npos.x() , npos.y() , nsize.x() , nsize.y() );
		glBindVertexArray( _vao[4] );
		glDrawElements( GL_TRIANGLES , 6 , GL_UNSIGNED_SHORT , 0 );

		npos = pos + f2( size.x() , size.y() );
		nsize = f2( border , border );
		glUniform4f( 5 , npos.x() , npos.y() , nsize.x() , nsize.y() );
		glBindVertexArray( _vao[5] );
		glDrawElements( GL_TRIANGLES , 6 , GL_UNSIGNED_SHORT , 0 );

		npos = pos + f2( 0.0f , size.y() );
		nsize = f2( size.x() - border , border );
		glUniform4f( 5 , npos.x() , npos.y() , nsize.x() , nsize.y() );
		glBindVertexArray( _vao[6] );
		glDrawElements( GL_TRIANGLES , 6 , GL_UNSIGNED_SHORT , 0 );

		npos = pos + f2( -size.x() , size.y() );
		nsize = f2( border , border );
		glUniform4f( 5 , npos.x() , npos.y() , nsize.x() , nsize.y() );
		glBindVertexArray( _vao[7] );
		glDrawElements( GL_TRIANGLES , 6 , GL_UNSIGNED_SHORT , 0 );

		npos = pos + f2( -size.x() , 0.0f );
		nsize = f2( border , size.y() - border );
		glUniform4f( 5 , npos.x() , npos.y() , nsize.x() , nsize.y() );
		glBindVertexArray( _vao[8] );
		glDrawElements( GL_TRIANGLES , 6 , GL_UNSIGNED_SHORT , 0 );
	}
	void release()
	{
		if( !isInited() ) return;
		setInited( false );
		_field_tex.release();
		glDeleteVertexArrays( 9 , _vao );
	}
	~PanelDrawler()
	{
		release();
	}
	static PanelDrawler *getSingleton()
	{
		static PanelDrawler sng[1];
		return sng;
	}
};
class TextRenderer : public RInitable
{
private:
	TTF_Font *_font;
	TTF_Font *loadfont( char *file , int ptsize )
	{
		TTF_Font* tmpfont;
		tmpfont = TTF_OpenFont( file , ptsize );
		if( tmpfont == NULL )
		{
			printf( "Unable to load font: %s %s \n" , file , TTF_GetError() );
		}
		return tmpfont;
	}
	SDL_Surface *drawtext( std::string text )
	{
		SDL_Color color = { 0 , 0 , 0 , 255 };
		//SDL_Color tmpfontbgcolor = { 0 , 0 , 0 , 0 };
		SDL_Surface *resulting_text;
		resulting_text = TTF_RenderText_Blended( _font , text.c_str() , color );
		return resulting_text;
	}
public:
	void init()
	{
		if( isInited() ) return;
		setInited( true );
		if( TTF_Init() )
		{
			LOG << "TTF init error";
		}
		_font = loadfont( "res/view/fonts/arial.ttf" , 32 );
		TTF_SetFontStyle( _font , TTF_STYLE_NORMAL );
	}
	std::unique_ptr< RImage[] > renderText( std::string text )
	{
		SDL_Surface *srf = drawtext( text );
		/*IMG_Init( IMG_INIT_JPG );
		SDL_SaveBMP( srf , "out.bmp" );
		IMG_Quit();*/
		uint bpp = srf->format->BytesPerPixel;
		//LOG << bpp << " " << srf->w << " " << srf->h << "\n";
		std::unique_ptr< char[] > data( new char[srf->w * srf->h * bpp] );
		memcpy( data.get() , srf->pixels , srf->w * srf->h * bpp );
		std::unique_ptr< RImage[] > out( new RImage[1]{ { std::move( data ) , { uint( srf->w ) , uint( srf->h ) } , bpp } } );
		SDL_FreeSurface( srf );
		return std::move( out );
	}
	void release()
	{
		TTF_CloseFont( _font );
		TTF_Quit();
	}
	~TextRenderer()
	{
		release();
	}
	static TextRenderer *getSingleton()
	{
		static TextRenderer sing[1];
		return sing;
	}
};
class GUITextHolderGL : public RInitable
{
public:
	RTextureHolderGL _text_texture;
	std::string _text;
public:
	GUITextHolderGL() = default;
	GUITextHolderGL( const GUITextHolderGL & ) = delete;
	void operator=( const GUITextHolderGL & ) = delete;
	GUITextHolderGL( GUITextHolderGL &&a ):
	_text( std::move( a._text ) )
	, _text_texture( std::move( a._text_texture ) )
	{
		setInited( a.isInited() );
		a.setInited( false );
	}
	void operator=( GUITextHolderGL &&a )
	{
		_text = std::move( a._text );
		_text_texture = std::move( a._text_texture );
		setInited( a.isInited() );
		a.setInited( false );
	}
	void init( std::string text )
	{
		if( isInited() ) return;
		setInited( true );
		_text = text;
		_text_texture = std::move( RTextureHolderGL( TextRenderer::getSingleton()->renderText( _text ) , 1 ) );
		_text_texture.init();
	}
	uint getTextureId() const
	{
		return _text_texture.getTexture();
	}
	void release()
	{
		if( !isInited() ) return;
		setInited( false );
		_text_texture.release();
		_text.clear();
	}
	~GUITextHolderGL()
	{
		release();
	}
};
class GUIRendererGL : public RInitable
{
private:
	RTextureHolderGL _panel_texture;
	RGraphicProgrammGL _text_shader , _panel_shader;
	RPolyQuadGL _gui_quad;
	char chars_per_row = 16;
	std::vector< GUITextHolderGL > _text_vector;
	uint genText( std::string const text )
	{
		for( int i = 0; i < _text_vector.size(); i ++ )
		{
			if( _text_vector[ i ]._text == text )
				return i;
		}
		GUITextHolderGL temp;
		temp.init( text );
		_text_vector.push_back( std::move( temp ) );
		return _text_vector.size() - 1;
	}
	void drawText( uint i , f2 const &pos , f2 const &size )
	{
		glEnable( GL_BLEND );
		glDisable( GL_DEPTH_TEST );
		glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
		//glBlendEquation( GL_FUNC_ADD );
		_text_shader.bind();
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D , _text_vector[ i ].getTextureId() );
		glUniform1i( 0 , 0 );
		glUniform4f( 5 , pos.x() , pos.y() , size.x() , size.y() );
		_gui_quad.draw();
		glDisable( GL_BLEND );
		glEnable( GL_DEPTH_TEST );
	}
	void drawPanel( f2 const &pos , f2 const &size , int level )
	{
		glEnable( GL_BLEND );
		glDisable( GL_DEPTH_TEST );
		glBlendFunc( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA );
		//glBlendFunc( GL_SRC_COLOR , GL_ONE_MINUS_SRC_COLOR );
		//glBlendEquation( GL_FUNC_ADD );
		_panel_shader.bind();
		glUniform4f( 5 , pos.x() , pos.y() , size.x() , size.y() );
		glActiveTexture( GL_TEXTURE0 + 1 );
		glBindTexture( GL_TEXTURE_2D , _panel_texture.getTexture() );
		glUniform1i( 1 , 1 );
		glUniform1f( 2 , float( level ) );
		//_gui_quad.draw();
		PanelDrawler::getSingleton()->drawPanel( pos , size , 0.02f );
		glDisable( GL_BLEND );
		glEnable( GL_DEPTH_TEST );
	}
public:
	GUIRendererGL( const GUIRendererGL & ) = delete;
	void operator=( const GUIRendererGL & ) = delete;
	GUIRendererGL() = default;
	void init()
	{
		if( isInited() ) return;
		setInited( true );
		TextRenderer::getSingleton()->init();
		PanelDrawler::getSingleton()->init();
		_gui_quad.init();
		_text_shader.init( "res/shaders/glsl/text_shader.glsl" , "res/shaders/glsl/gui_vertex.glsl" );
		_panel_shader.init( "res/shaders/glsl/panel_shader.glsl" , "res/shaders/glsl/gui_vertex.glsl" );
		_panel_texture.init( std::move( RFileLoader::loadImage( "res/view/images/panel.png" ) ) , 1 );
		_panel_texture.setRepeat( false );
	}
	void renderLayout( int w , int h , GUILayout const *layout )
	{
		const float border = 0.01f;
		float ltop = border , lbot = border , rtop = border , rbot = border , ctop = border , cbot = border , ccenter = 0.5f;
		for( auto const &i : layout->getElemVec() )
		{
			switch( i->_floatX )
			{
				case GUIElem::GUIFloat::LEFT:
				{
					switch( i->_floatY )
					{
						case GUIElem::GUIFloat::TOP:
						{
							f2 pos = f2( i->_size_pix.x() * 0.5f / w + border , ltop + i->_size_pix.y() * 0.5f / h );
							ltop += i->_size_pix.y() / h + border;
							i->_calculated_pos = f2( pos.x() *  2.0f - 1.0f , pos.y() * -2.0f + 1.0 );
							i->_calculated_size = i->_size_pix / f2( w , h );
						}
						break;
						case GUIElem::GUIFloat::BOTTOM:
						{
							f2 pos = f2( i->_size_pix.x() * 0.5f / w + border , 1.0f - lbot - i->_size_pix.y() * 0.5f / h );
							lbot += i->_size_pix.y() / h + border;
							i->_calculated_pos = f2( pos.x() *  2.0f - 1.0f , pos.y() * -2.0f + 1.0 );
							i->_calculated_size = i->_size_pix / f2( w , h );
						}
						break;
						case GUIElem::GUIFloat::CENTER:
						{

						}
						break;
					}
				}
				break;
				case GUIElem::GUIFloat::RIGHT:
				{
					switch( i->_floatY )
					{
						case GUIElem::GUIFloat::TOP:
						{
							f2 pos = f2( 1.0f - i->_size_pix.x() * 0.5f / w - border , rtop + i->_size_pix.y() * 0.5f / h );
							rtop += i->_size_pix.y() / h + border;
							i->_calculated_pos = f2( pos.x() *  2.0f - 1.0f , pos.y() * -2.0f + 1.0 );
							i->_calculated_size = i->_size_pix / f2( w , h );
						}
						break;
						case GUIElem::GUIFloat::BOTTOM:
						{
							f2 pos = f2( 1.0f - i->_size_pix.x() * 0.5f / w - border , 1.0f - rbot - i->_size_pix.y() * 0.5f / h );
							rbot += i->_size_pix.y() / h + border;
							i->_calculated_pos = f2( pos.x() *  2.0f - 1.0f , pos.y() * -2.0f + 1.0 );
							i->_calculated_size = i->_size_pix / f2( w , h );
						}
						break;
						case GUIElem::GUIFloat::CENTER:
						{

						}
						break;
					}
				}
				break;
				case GUIElem::GUIFloat::CENTER:
				{
					switch( i->_floatY )
					{
						case GUIElem::GUIFloat::TOP:
						{

						}
						break;
						case GUIElem::GUIFloat::BOTTOM:
						{

						}
						break;
						case GUIElem::GUIFloat::CENTER:
						{
							f2 pos = f2( 0.5f , 1.0f - ccenter - i->_size_pix.y() * 0.5f / h );
							ccenter += i->_size_pix.y() / h + border;
							i->_calculated_pos = f2( pos.x() *  2.0f - 1.0f , pos.y() * -2.0f + 1.0 );
							i->_calculated_size = i->_size_pix / f2( w , h );
						}
						break;
					}
				}
				break;
			}
			drawPanel( i->_calculated_pos , i->_calculated_size , i->_status );
			drawText( genText( i->_text ) , i->_calculated_pos , i->_calculated_size * 0.6f );
		}
	}
	void release()
	{
		if( !isInited() ) return;
		setInited( false );
		TextRenderer::getSingleton()->release();
		PanelDrawler::getSingleton()->release();
		_panel_texture.release();
		_gui_quad.release();
		_text_shader.release();
		_panel_shader.release();
		_text_vector.clear();
	}
	~GUIRendererGL()
	{
		release();
	}
};
#endif // GIRENDERERGL_H
