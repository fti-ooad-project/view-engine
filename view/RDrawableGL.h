#ifndef RDRAWABLEGL_H
#define RDRAWABLEGL_H
#include "RDrawable.h"
#include "GL.h"
#include "RAnimGL.h"
#include "RTexturesGL.h"
class RPolyMeshGL : public RDrawable
{
	uint _vao;
	int _indx_count;
	virtual void bindRes( RDrawableState const & ) const = 0;
	void draw( RDrawableState const &stat ) const override
	{
		bindRes( stat );
		glBindVertexArray( _vao );
		glDrawElements( GL_TRIANGLES , _indx_count , GL_UNSIGNED_SHORT , 0 );
	}
	void drawPatches( RDrawableState const &stat ) const override
	{
		bindRes( stat );
		glBindVertexArray( _vao );
		glDrawElements( GL_PATCHES , _indx_count , GL_UNSIGNED_SHORT , 0 );
	}
	void release() override
	{
		glDeleteVertexArrays( 1 , &_vao );
	}
};
class RComplexPolyMeshGL : public RPolyMeshGL
{
public:
	RTextureHolderGL __textures;
	RBoneAnimInTexHolderGL __anim_intex;
	std::unique_ptr< RPolymesh > __mesh;
	RComplexPolyMeshGL( std::unique_ptr< RPolymesh > &&mesh ):
	__textures( std::move( mesh->_textures ) , mesh->_texture_count )
	, __anim_intex( std::move( mesh->__mat4anim ) , mesh->_anim_count )
	, __mesh( std::move( mesh ) )///ORDER
	{
	}
	void init()
	{
		_flags = __mesh->_flags;
		glGenVertexArrays( 1 , &_vao );
		glBindVertexArray( _vao );
		uint vbo , ibo;
		glGenBuffers( 1 , &vbo );
		glBindBuffer( GL_ARRAY_BUFFER_ARB , vbo );
		switch( __mesh->_type ){
			case RPolymesh::RPolyMeshType::RBONED_PMESH:
			{
				glBufferData( GL_ARRAY_BUFFER_ARB ,
											__mesh->_vertex_count * RVertexOffsets::Base , __mesh->__vertices.get() ,
											GL_STATIC_DRAW_ARB );
				glEnableVertexAttribArray( 0 );
				glEnableVertexAttribArray( 1 );
				glEnableVertexAttribArray( 2 );
				glEnableVertexAttribArray( 3 );
				glEnableVertexAttribArray( 4 );
				glEnableVertexAttribArray( 5 );
				glEnableVertexAttribArray( 6 );
				glVertexAttribPointer( 0 , 3 , GL_FLOAT , RVertexOffsets::Base , 0 );
				glVertexAttribPointer( 2 , 3 , GL_FLOAT , RVertexOffsets::Base , RVertexOffsets::Normal );
				glVertexAttribPointer( 3 , 3 , GL_FLOAT , RVertexOffsets::Base , RVertexOffsets::BiNormal );
				glVertexAttribPointer( 4 , 3 , GL_FLOAT , RVertexOffsets::Base , RVertexOffsets::TangentNormal );
				glVertexAttribPointer( 1 , 2 , GL_FLOAT , RVertexOffsets::Base , RVertexOffsets::TextureCoordinate );
				glVertexAttribPointer( 5 , 3 , GL_FLOAT , RVertexOffsets::Base , RVertexOffsets::BoneWeights );
				glVertexAttribPointer( 6 , 4 , GL_FLOAT , RVertexOffsets::Base , RVertexOffsets::BoneIndex );
			}
			break;
		}
		glGenBuffers( 1 , &ibo );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB , ibo );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER_ARB ,
				3 * __mesh->_face_count * sizeof(unsigned short) ,
				__mesh->__indeces.get() , GL_STATIC_DRAW_ARB );
		_indx_count = 3 * __mesh->_face_count;
		glBindVertexArray( 0 );
		/*glGenTextures( __mesh->_textures._count , __textures );
		for ( int i = 0; i < __mesh->_textures._count; i++ )
		{
			glBindTexture( GL_TEXTURE_2D , __textures[i] );
			glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA , __mesh->_textures.__img[i]._size._w , __mesh->_textures.__img[i]._size._h , 0 ,
					GL_RGBA , GL_UNSIGNED_BYTE , __mesh->_textures.__img[i].__data.get() );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER ,
					GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER ,
					GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S ,
					GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T ,
					GL_REPEAT );
		}*/

		__anim_intex.init();
		__textures.init();
#ifdef RLOG
		LOG << "_________________________\n" << "polymesh generated:vao:" << _vao << "\n";
#endif
		__mesh.reset();
	}
	void release() override
	{
		glDeleteVertexArrays( 1 , &_vao );
		__anim_intex.release();
		__textures.release();
	}
	~RComplexPolyMeshGL()
	{
		release();
	}
	void bindRes( RDrawableState const &stat ) const override
	{
		if( _flags & ShaderMask::MASK_ANIMATED )
		{
			glActiveTexture( GL_TEXTURE0 + 3 );
			glBindTexture( GL_TEXTURE_2D , __anim_intex.__texture_pointer_array[stat._animstat._moment._cur_set] );
			glUniform1i( 20 , 3 );///20-location of anim texture
			glActiveTexture( GL_TEXTURE0 + 4 );
			glBindTexture( GL_TEXTURE_2D , __anim_intex.__texture_pointer_array[stat._animstat._moment._last_set] );
			glUniform1i( 21 , 4 );///21-location of anim texture
			glUniform1f( 7 , stat._animstat._moment._time );///same
			glUniform1i( 8 , static_cast< int >( stat._animstat._moment._mix ) );
			glUniform1f( 9 , stat._animstat._moment._last_time );
			glUniform1f( 10 , __anim_intex._bone_count );
		}
		if( _flags & ShaderMask::MASK_TEXTURED )
		{
			ito( __textures._count )///<=3
			{
				glActiveTexture( GL_TEXTURE0 + i );
				glBindTexture( GL_TEXTURE_2D , __textures.__texture_pointer_array[i] );
				glUniform1i( 1 + i , i );///1-layout of the first texture in shader
			}
		}
	}
};
struct RPolyQuadGL : public RPolyMeshGL
{
	void init()
	{
		static constexpr float quad[] =
		{
			-1.0f , -1.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 1.0f ,
			1.0f , -1.0f , 0.0f , 1.0f , 0.0f , 0.0f , 0.0f , 1.0f ,
			1.0f , 1.0f , 0.0f , 1.0f , 1.0f , 0.0f , 0.0f , 1.0f ,
			-1.0f , 1.0f , 0.0f , 0.0f , 1.0f , 0.0f , 0.0f , 1.0f
		};
		static constexpr ushort indx[] =
		{
			0 , 1 , 2 , 0 , 2 , 3
		};
		glGenVertexArrays( 1 , &_vao );
		glBindVertexArray( _vao );
		uint vbo , ibo;
		glGenBuffers( 1 , &vbo );
		glBindBuffer( GL_ARRAY_BUFFER_ARB , vbo );
		glBufferData( GL_ARRAY_BUFFER_ARB , 128 , quad , GL_STATIC_DRAW_ARB );
		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );
		glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , 32 , 0 );
		glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , 32 , 12 );
		glVertexAttribPointer( 2 , 3 , GL_FLOAT , GL_FALSE , 32 , 20 );
		glGenBuffers( 1 , &ibo );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB , ibo );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER_ARB , 12 , indx , GL_STATIC_DRAW_ARB );
		glBindVertexArray( 0 );
		glDeleteBuffers( 1 , &vbo );
		glDeleteBuffers( 1 , &ibo );
		_indx_count = 6;
	}
	~RPolyQuadGL()
	{
		release();
	}
	void bindRes( RDrawableState const &stat ) override
	{

	}
};
struct RPolyBoxGL : public RPolyMeshGL
{
	void init()
	{
		static constexpr float quad[] =
		{
			-1.0f , -1.0f , 1.0f , 0.0f , 0.0f , -0.6f , -0.6f , 0.6f ,
			1.0f , -1.0f , 1.0f , 1.0f , 0.0f , 0.6f , -0.6f , 0.6f ,
			1.0f , 1.0f , 1.0f , 1.0f , 1.0f , 0.6f , 0.6f , 0.6f ,
			-1.0f , 1.0f , 1.0f , 0.0f , 1.0f , -0.6f , 0.6f , 0.6f ,///
			-1.0f , -1.0f , -1.0f , 0.0f , 0.0f , -0.6f , -0.6f , -0.6f ,
			1.0f , -1.0f , -1.0f , 1.0f , 0.0f , 0.6f , -0.6f , -0.6f ,
			1.0f , 1.0f , -1.0f , 1.0f , 1.0f , 0.6f , 0.6f , -0.6f ,
			-1.0f , 1.0f , -1.0f , 0.0f , 1.0f , -0.6f , 0.6f , -0.6f ///
		};
		static constexpr ushort indx[] =
		{
			0 , 2 , 3 , 0 , 1 , 2 ,
			4 , 7 , 6 , 4 , 6 , 5 ,
			1 , 6 , 2 , 1 , 5 , 6 ,
			0 , 3 , 7 , 0 , 7 , 4 ,
			2 , 7 , 3 , 2 , 6 , 7 ,
			1 , 0 , 4 , 1 , 4 , 5
		};
		glGenVertexArrays( 1 , &_vao );
		glBindVertexArray( _vao );
		uint vbo , ibo;
		glGenBuffers( 1 , &vbo );
		glBindBuffer( GL_ARRAY_BUFFER_ARB , vbo );
		glBufferData( GL_ARRAY_BUFFER_ARB , 256 , quad , GL_STATIC_DRAW_ARB );
		glEnableVertexAttribArray( 0 );
		glEnableVertexAttribArray( 1 );
		glEnableVertexAttribArray( 2 );
		glVertexAttribPointer( 0 , 3 , GL_FLOAT , 32 , 0 );
		glVertexAttribPointer( 1 , 2 , GL_FLOAT , 32 , 12 );
		glVertexAttribPointer( 2 , 3 , GL_FLOAT , 32 , 20 );
		glGenBuffers( 1 , &ibo );
		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB , ibo );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER_ARB , 72 , indx , GL_STATIC_DRAW_ARB );
		glBindVertexArray( 0 );
		glDeleteBuffers( 1 , &vbo );
		glDeleteBuffers( 1 , &ibo );
		_indx_count = 36;
	}
	~RPolyBoxGL()
	{
		release();
	}
	void bindRes( RDrawableState const &stat ) override
	{

	}
};
#endif // RDRAWABLEGL_H
