#ifndef RDRAWABLEGL_H
#define RDRAWABLEGL_H
#include "RDrawable.h"
#include "../GL.h"
#include "RAnimGL.h"
#include "RTexturesGL.h"
#include "ShaderSpecs.h"
#include "RVertex.h"
struct InstanceInfo
{
	float time;
	float last_time;
	int mixing;
	int cur_anim;
	int last_anim;
	f4x4 model;
};
class RPolyMeshGL : public RInitable
{
protected:
	uint _vao;
	uint _instanced_buf = 0;
	int _indx_count;
public:
	int _flags = 0;
public:
	bool isInstanced() const
	{
		return bool( _instanced_buf );
	}
	void genInstancedBuffer()
	{
		if( _instanced_buf ) return;
		glBindVertexArray( _vao );
		glGenBuffers( 1 , &_instanced_buf );
		glBindBuffer( GL_ARRAY_BUFFER , _instanced_buf );
		uint datasize = sizeof( InstanceInfo );
		int mc = 12;
		for( int c = 7; c <= mc; ++c )
			glEnableVertexAttribArray( c );
		glVertexAttribPointer( 7 , 2 , GL_FLOAT , GL_FALSE , datasize , reinterpret_cast< void* >( 0 ) );
		glVertexAttribPointer( 8 , 3 , GL_FLOAT , GL_FALSE , datasize , reinterpret_cast< void* >( 8 ) );
		glVertexAttribPointer( 9 , 4 , GL_FLOAT , GL_FALSE , datasize , reinterpret_cast< void* >( 20 ) );
		glVertexAttribPointer( 10 , 4 , GL_FLOAT , GL_FALSE , datasize , reinterpret_cast< void* >( 36 ) );
		glVertexAttribPointer( 11 , 4 , GL_FLOAT , GL_FALSE , datasize , reinterpret_cast< void* >( 52 ) );
		glVertexAttribPointer( 12 , 4 , GL_FLOAT , GL_FALSE , datasize , reinterpret_cast< void* >( 68 ) );
		for( int c = 7; c <= mc; ++c )
			glVertexAttribDivisor( c , 1 );
		glBindBuffer( GL_ARRAY_BUFFER , 0 );
		glBindVertexArray( 0 );
		//LOG<<datasize;
	}
	virtual void bindRes( InstanceInfo const & ) const = 0;
	void draw( InstanceInfo const &stat ) const
	{
		bindRes( stat );
		glBindVertexArray( _vao );
		glDrawElements( GL_TRIANGLES , _indx_count , GL_UNSIGNED_SHORT , 0 );
	}
	void draw() const
	{
		glBindVertexArray( _vao );
		glDrawElements( GL_TRIANGLES , _indx_count , GL_UNSIGNED_SHORT , 0 );
	}
	void drawPatches( InstanceInfo const &stat ) const
	{
		bindRes( stat );
		glBindVertexArray( _vao );
		glDrawElements( GL_PATCHES , _indx_count , GL_UNSIGNED_SHORT , 0 );
	}
	void release()
	{
		glDeleteVertexArrays( 1 , &_vao );
		glDeleteBuffers( 1 , &_instanced_buf );
	}
	void drawInstanced( std::vector< InstanceInfo > const &data )
	{
		bindRes( data[0] );
		glBindVertexArray( _vao );
		glBindBuffer( GL_ARRAY_BUFFER , _instanced_buf );
		glBufferData( GL_ARRAY_BUFFER , sizeof( InstanceInfo ) * data.size() , &data[0] , GL_DYNAMIC_DRAW );
		glDrawElementsInstanced( GL_TRIANGLES , _indx_count , GL_UNSIGNED_SHORT , 0 , data.size() );
		glBindBuffer( GL_ARRAY_BUFFER , 0 );
		glBindVertexArray( 0 );
		//data[0].pos.print();
		//LOG << _instanced_buf;
	}
};
class RComplexPolyMeshGL : public RPolyMeshGL
{
public:
	RTextureHolderGL __textures;
	RBoneAnimInTexHolderGL __anim_intex;
	std::unique_ptr< RPolymesh > __mesh;
	int _bone_count;
	RComplexPolyMeshGL( std::unique_ptr< RPolymesh > &&mesh ):
	__textures( std::move( mesh->_textures ) , mesh->_texture_count )
	, __anim_intex( std::move( mesh->__mat4anim ) , mesh->_anim_count )
	, __mesh( std::move( mesh ) )///ORDER
	, _bone_count( __mesh->_bone_count )
	{
	}
	void init() override
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
				glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , RVertexOffsets::Base , reinterpret_cast< void* >( 0 ) );
				glVertexAttribPointer( 2 , 3 , GL_FLOAT , GL_FALSE , RVertexOffsets::Base , reinterpret_cast< void* >( RVertexOffsets::Normal ) );
				glVertexAttribPointer( 3 , 3 , GL_FLOAT , GL_FALSE , RVertexOffsets::Base , reinterpret_cast< void* >( RVertexOffsets::BiNormal ) );
				glVertexAttribPointer( 4 , 3 , GL_FLOAT , GL_FALSE , RVertexOffsets::Base , reinterpret_cast< void* >( RVertexOffsets::TangentNormal ) );
				glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , RVertexOffsets::Base , reinterpret_cast< void* >( RVertexOffsets::TextureCoordinate ) );
				glVertexAttribPointer( 5 , 3 , GL_FLOAT , GL_FALSE , RVertexOffsets::Base , reinterpret_cast< void* >( RVertexOffsets::BoneWeights ) );
				glVertexAttribPointer( 6 , 4 , GL_FLOAT , GL_FALSE , RVertexOffsets::Base , reinterpret_cast< void* >( RVertexOffsets::BoneIndex ) );
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
	void bindRes( InstanceInfo const &stat ) const override
	{
		glUniform1i( 0 , _flags );
		if( _flags & ShaderMask::MASK_ANIMATED )
		{
			glUniform1f( 7 , stat.time );///same
			glUniform1i( 8 , stat.mixing );
			glUniform1f( 9 , stat.last_time );
			glUniform1i( 10 , _bone_count );
			glUniform1i( 13 , stat.cur_anim );
			glUniform1f( 14 , stat.last_anim );
		}
		int c = 0;
		if( ( _flags & ShaderMask::MASK_TEXTURED ) && __textures._count )
		{
			ito( __textures._count )///<=3
			{
				glActiveTexture( GL_TEXTURE0 + c );
				glBindTexture( GL_TEXTURE_2D , __textures.__texture_pointer_array[i] );
				glUniform1i( 1 + i , c );///1-layout of the first texture in shader
				c++;
			}
		}
		if( ( _flags & ShaderMask::MASK_OWN_ANIMATED ) && __anim_intex._count )
		{
			ito( __anim_intex._count )
			{
				glActiveTexture( GL_TEXTURE0 + c );
				glBindTexture( GL_TEXTURE_2D , __anim_intex.__texture_pointer_array[i] );
				glUniform1i( 20 + i , c );///20-location of anim texture
				c++;
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
			-1.0f , -1.0f , 0.0f , 0.0f , 1.0f , 0.0f , 0.0f , 1.0f ,
			1.0f , -1.0f , 0.0f , 1.0f , 1.0f , 0.0f , 0.0f , 1.0f ,
			1.0f , 1.0f , 0.0f , 1.0f , 0.0f , 0.0f , 0.0f , 1.0f ,
			-1.0f , 1.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 1.0f
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
		glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , 32 , reinterpret_cast< void* >( 0 ) );
		glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , 32 , reinterpret_cast< void* >( 12 ) );
		glVertexAttribPointer( 2 , 3 , GL_FLOAT , GL_FALSE , 32 , reinterpret_cast< void* >( 20 ) );
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
	void bindRes( InstanceInfo const &stat ) const override
	{
		glUniform1i( 0 , _flags );
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
		glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , 32 , reinterpret_cast< void* >( 0 ) );
		glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , 32 , reinterpret_cast< void* >( 12 ) );
		glVertexAttribPointer( 2 , 3 , GL_FLOAT , GL_FALSE , 32 , reinterpret_cast< void* >( 20 ) );
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
	void bindRes( InstanceInfo const &stat ) const override
	{
		glUniform1i( 0 , _flags );
	}
};
#endif // RDRAWABLEGL_H
