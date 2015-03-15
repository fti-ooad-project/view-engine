#include "../view/3dgl/RDrawableGL.h"
bool RPolyMeshGL::isInstanced() const
{
	return bool( _instanced_buf );
}
void RPolyMeshGL::genInstancedBuffer()
{
	if( _instanced_buf ) return;
	glBindVertexArray( _vao );
	glGenBuffers( 1 , &_instanced_buf );
	glBindBuffer( GL_ARRAY_BUFFER , _instanced_buf );
	uint datasize = sizeof( InstanceInfo );
	int mc = 12;
	for( int c = 7; c <= mc; ++c )
		glEnableVertexAttribArray( c );
	glVertexAttribPointer( 7 , 3 , GL_FLOAT , GL_FALSE , datasize , reinterpret_cast< void* >( 0 ) );
	glVertexAttribPointer( 8 , 3 , GL_FLOAT , GL_FALSE , datasize , reinterpret_cast< void* >( 12 ) );
	glVertexAttribPointer( 9 , 4 , GL_FLOAT , GL_FALSE , datasize , reinterpret_cast< void* >( 24 ) );
	glVertexAttribPointer( 10 , 4 , GL_FLOAT , GL_FALSE , datasize , reinterpret_cast< void* >( 40 ) );
	glVertexAttribPointer( 11 , 4 , GL_FLOAT , GL_FALSE , datasize , reinterpret_cast< void* >( 56 ) );
	glVertexAttribPointer( 12 , 4 , GL_FLOAT , GL_FALSE , datasize , reinterpret_cast< void* >( 72 ) );
	for( int c = 7; c <= mc; ++c )
		glVertexAttribDivisor( c , 1 );
	glBindBuffer( GL_ARRAY_BUFFER , 0 );
	glBindVertexArray( 0 );
	//LOG<<datasize;
}
void RPolyMeshGL::draw( InstanceInfo const &stat ) const
{
	bindRes( stat );
	glBindVertexArray( _vao );
	glDrawElements( GL_TRIANGLES , _indx_count , GL_UNSIGNED_SHORT , 0 );
}
void RPolyMeshGL::draw() const
{
	glBindVertexArray( _vao );
	glDrawElements( GL_TRIANGLES , _indx_count , GL_UNSIGNED_SHORT , 0 );
}
void RPolyMeshGL::drawPatches( InstanceInfo const &stat ) const
{
	bindRes( stat );
	glBindVertexArray( _vao );
	glDrawElements( GL_PATCHES , _indx_count , GL_UNSIGNED_SHORT , 0 );
}
void RPolyMeshGL::release()
{
	glDeleteVertexArrays( 1 , &_vao );
	glDeleteBuffers( 1 , &_instanced_buf );
}
void RPolyMeshGL::drawInstanced( std::vector< InstanceInfo > const &data )
{
	bindRes( data[ 0 ] );
	glBindVertexArray( _vao );
	glBindBuffer( GL_ARRAY_BUFFER , _instanced_buf );
	glBufferData( GL_ARRAY_BUFFER , sizeof( InstanceInfo ) * data.size() , &data[ 0 ] , GL_DYNAMIC_DRAW );
	glDrawElementsInstanced( GL_TRIANGLES , _indx_count , GL_UNSIGNED_SHORT , 0 , data.size() );
	glBindBuffer( GL_ARRAY_BUFFER , 0 );
	glBindVertexArray( 0 );
	//data[0].pos.print();
	//LOG << _instanced_buf;
}
void RPolyMeshGL::drawInstancedPatches( std::vector< InstanceInfo > const &data )
{
	bindRes( data[ 0 ] );
	glBindVertexArray( _vao );
	glBindBuffer( GL_ARRAY_BUFFER , _instanced_buf );
	glBufferData( GL_ARRAY_BUFFER , sizeof( InstanceInfo ) * data.size() , &data[ 0 ] , GL_DYNAMIC_DRAW );
	glDrawElementsInstanced( GL_PATCHES , _indx_count , GL_UNSIGNED_SHORT , 0 , data.size() );
	glBindBuffer( GL_ARRAY_BUFFER , 0 );
	glBindVertexArray( 0 );
	//data[0].pos.print();
	//LOG << _instanced_buf;
}
RComplexPolyMeshGL::RComplexPolyMeshGL( RComplexPolyMeshGL &&mesh )
{
	*this = std::move( mesh );
}
void RComplexPolyMeshGL::operator=( RComplexPolyMeshGL &&mesh )
{
	release();
	setInited( mesh.isInited() );
	if( mesh.isInited() )
	{
		if( mesh.__textures.getCount() > 0 )
			__textures = std::move( mesh.__textures );
		if( mesh.__anim_intex.getCount() > 0 )
			__anim_intex = std::move( mesh.__anim_intex );
		_vao = mesh._vao;
		_instanced_buf = mesh._instanced_buf;
		_bone_count = mesh._bone_count;
		_size = mesh._size;
	} else
	{
		__mesh = std::move( mesh.__mesh );
		_bone_count = __mesh->_bone_count;
		_size = __mesh->_v3size * 2.0f;
	}
}
RComplexPolyMeshGL::RComplexPolyMeshGL( std::unique_ptr< RPolymesh > &&mesh )
{
	__mesh = std::move( mesh );
	if( __mesh->_texture_count > 0 )
		__textures = std::move( RTextureHolderGL( std::move( __mesh->_textures ) , __mesh->_texture_count ) );
	if( __mesh->_anim_count > 0 )
		__anim_intex = std::move( RBoneAnimInTexHolderGL( std::move( __mesh->__mat4anim ) , __mesh->_anim_count ) );
	_bone_count = __mesh->_bone_count;
	_size = __mesh->_v3size * 2.0f;
}
void RComplexPolyMeshGL::init( std::unique_ptr< RPolymesh > &&mesh )
{
	*this = std::move( RComplexPolyMeshGL( std::move( mesh ) ) );
	init();
}
void RComplexPolyMeshGL::init()
{
	if( !__mesh ) return;
	if( isInited() ) return;
	setInited( true );
	_flags = __mesh->_flags;
	glGenVertexArrays( 1 , &_vao );
	glBindVertexArray( _vao );
	uint vbo , ibo;
	glGenBuffers( 1 , &vbo );
	glBindBuffer( GL_ARRAY_BUFFER_ARB , vbo );
	switch( __mesh->_type )
	{
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
			__anim_intex.init();
		}
		break;
		case RPolymesh::RPolyMeshType::RSTATIC_PMESH:
		{
			glBufferData( GL_ARRAY_BUFFER_ARB ,
						  __mesh->_vertex_count * RStaticVertexOffsets::Base , __mesh->__vertices.get() ,
						  GL_STATIC_DRAW_ARB );
			glEnableVertexAttribArray( 0 );
			glEnableVertexAttribArray( 1 );
			glEnableVertexAttribArray( 2 );
			glEnableVertexAttribArray( 3 );
			glEnableVertexAttribArray( 4 );
			glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , RStaticVertexOffsets::Base , reinterpret_cast< void* >( 0 ) );
			glVertexAttribPointer( 2 , 3 , GL_FLOAT , GL_FALSE , RStaticVertexOffsets::Base , reinterpret_cast< void* >( RStaticVertexOffsets::Normal ) );
			glVertexAttribPointer( 3 , 3 , GL_FLOAT , GL_FALSE , RStaticVertexOffsets::Base , reinterpret_cast< void* >( RStaticVertexOffsets::BiNormal ) );
			glVertexAttribPointer( 4 , 3 , GL_FLOAT , GL_FALSE , RStaticVertexOffsets::Base , reinterpret_cast< void* >( RStaticVertexOffsets::TangentNormal ) );
			glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , RStaticVertexOffsets::Base , reinterpret_cast< void* >( RStaticVertexOffsets::TextureCoordinate ) );
		}
		break;
	}
	glGenBuffers( 1 , &ibo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB , ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER_ARB ,
				  3 * __mesh->_face_count * sizeof( unsigned short ) ,
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
	__textures.init();
#ifdef RLOG
	LOG << "_________________________\n" << "polymesh generated:vao:" << _vao << "\n";
#endif
	__mesh.reset();
}
void RComplexPolyMeshGL::release()
{
	if( !isInited() ) return;
	setInited( false );
	glDeleteVertexArrays( 1 , &_vao );
	__anim_intex.release();
	__textures.release();
}
RComplexPolyMeshGL::~RComplexPolyMeshGL()
{
	release();
}
void RComplexPolyMeshGL::bindRes( InstanceInfo const &stat ) const
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
	if( ( _flags & ShaderMask::MASK_TEXTURED ) && __textures.getCount() )
	{
		ito( __textures.getCount() )///<=3
		{
			glActiveTexture( GL_TEXTURE0 + c );
			glBindTexture( GL_TEXTURE_2D , __textures.getTexture( i ) );
			glUniform1i( 1 + i , c );///1-layout of the first texture in shader
			c++;
		}
	}
	if( ( _flags & ShaderMask::MASK_OWN_ANIMATED ) && __anim_intex.getCount() )
	{
		ito( __anim_intex.getCount() )
		{
			glActiveTexture( GL_TEXTURE0 + c );
			glBindTexture( GL_TEXTURE_2D , __anim_intex.getTexture( i ) );
			glUniform1i( 20 + i , c );///20-location of anim texture
			c++;
		}
	}
}
void RPolyQuadGL::init()
{
	static constexpr float quad[] =
	{
		-1.0f , -1.0f , 0.0f , 0.0f , 1.0f , 0.0f , 0.0f , 1.0f , /*bn*/1.0f , 0.0f , 0.0f , /*tn*/0.0f , 1.0f , 0.0f ,
		1.0f , -1.0f , 0.0f , 1.0f , 1.0f , 0.0f , 0.0f , 1.0f , /*bn*/1.0f , 0.0f , 0.0f , /*tn*/0.0f , 1.0f , 0.0f ,
		1.0f , 1.0f , 0.0f , 1.0f , 0.0f , 0.0f , 0.0f , 1.0f , /*bn*/1.0f , 0.0f , 0.0f , /*tn*/0.0f , 1.0f , 0.0f ,
		-1.0f , 1.0f , 0.0f , 0.0f , 0.0f , 0.0f , 0.0f , 1.0f , /*bn*/1.0f , 0.0f , 0.0f , /*tn*/0.0f , 1.0f , 0.0f
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
	glBufferData( GL_ARRAY_BUFFER_ARB , 224 , quad , GL_STATIC_DRAW_ARB );
	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );
	glEnableVertexAttribArray( 2 );
	glEnableVertexAttribArray( 3 );
	glEnableVertexAttribArray( 4 );
	glVertexAttribPointer( 0 , 3 , GL_FLOAT , GL_FALSE , 56 , reinterpret_cast< void* >( 0 ) );
	glVertexAttribPointer( 1 , 2 , GL_FLOAT , GL_FALSE , 56 , reinterpret_cast< void* >( 12 ) );
	glVertexAttribPointer( 2 , 3 , GL_FLOAT , GL_FALSE , 56 , reinterpret_cast< void* >( 20 ) );
	glVertexAttribPointer( 3 , 3 , GL_FLOAT , GL_FALSE , 56 , reinterpret_cast< void* >( 32 ) );
	glVertexAttribPointer( 4 , 3 , GL_FLOAT , GL_FALSE , 56 , reinterpret_cast< void* >( 44 ) );
	glGenBuffers( 1 , &ibo );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER_ARB , ibo );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER_ARB , 12 , indx , GL_STATIC_DRAW_ARB );
	glBindVertexArray( 0 );
	glDeleteBuffers( 1 , &vbo );
	glDeleteBuffers( 1 , &ibo );
	_indx_count = 6;
}
RPolyQuadGL::~RPolyQuadGL()
{
	release();
}
void RPolyQuadGL::bindRes( InstanceInfo const &stat ) const
{
	glUniform1i( 0 , _flags );
}
void RPolyBoxGL::init()
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
RPolyBoxGL::~RPolyBoxGL()
{
	release();
}
void RPolyBoxGL::bindRes( InstanceInfo const &stat ) const
{
	glUniform1i( 0 , _flags );
}