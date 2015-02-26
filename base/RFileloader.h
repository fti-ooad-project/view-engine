#ifndef __RFileloader_H__
#define __RFileloader_H__
#include <cstring>
#include "view/3dgl/RTextures.h"
#include "view/3dgl/RDrawable.h"
#include <string>
#include <stdexcept>
#include <memory>
#ifdef _WIN32
#include <conio.h>
#endif
#include <fstream>
#include <sstream>
//==========================
//	RData class 1.04.14 schreiner
//==========================
//#define ifor( x ) for( int i = 0; i < x; ++i )
class RFileLoader
{
public:
	static void loadImageBin( RImage &out ,std::shared_ptr< std::ifstream > stream )
	{
        stream->read( ( char * )&out._size._w , sizeof( int ) );
        stream->read( ( char * )&out._size._h , sizeof( int ) );
		out.__data = std::move( std::unique_ptr< char >( new char[4 * out._size._w * out._size._h] ) );
		stream->read( ( char * )out.__data.get() , 4 * out._size._w * out._size._h );
    }
	static std::unique_ptr< RPolymesh > loadPolyMeshBin( std::shared_ptr< std::ifstream > stream , int type )
    {
		std::unique_ptr< RPolymesh > out( new RPolymesh() );
		out->_type = type;
		switch( type )
		{
			case RPolymesh::RPolyMeshType::RBONED_PMESH:
				out->_flags |= ShaderMask::MASK_ANIMATED;
				stream->read( ( char * )&out->_v3size, sizeof( float ) * 3 );
				stream->read( ( char * )&out->_bone_count, sizeof( unsigned int ) );
				stream->read( ( char * )&out->_face_count, sizeof( unsigned int ) );
				stream->read( ( char * )&out->_vertex_count, sizeof( unsigned int ) );
				out->__vertices = std::move( std::unique_ptr< char[] >( reinterpret_cast< char* >( new RVertex[out->_vertex_count] ) ) );
				out->__indeces = std::move( std::unique_ptr< unsigned short[] >( new unsigned short[out->_face_count * 3] ) );
				stream->read( ( char * )out->__vertices.get(), sizeof( RVertex )*out->_vertex_count );
				//ito( out->_vertex_count )
				//	PRINT( reinterpret_cast< RVertex* >( out->__vertices.get() )[i].BoneIndex[0] );
				stream->read( ( char * )out->__indeces.get(), sizeof( unsigned short )*out->_face_count * 3 );
			break;
			case RPolymesh::RPolyMeshType::RSTATIC_PMESH:
				stream->read( ( char * )&out->_v3size, sizeof( float ) * 3 );
				stream->read( ( char * )&out->_face_count, sizeof( unsigned int ) );
				stream->read( ( char * )&out->_vertex_count, sizeof( unsigned int ) );
				out->__vertices = std::move( std::unique_ptr< char[] >( reinterpret_cast< char* >( new RStaticVertex[out->_vertex_count] ) ) );
				out->__indeces = std::move( std::unique_ptr< unsigned short[] >( new unsigned short[out->_face_count * 3] ) );
				stream->read( ( char * )out->__vertices.get(), sizeof( RVertex )*out->_vertex_count );
				stream->read( ( char * )out->__indeces.get(), sizeof( unsigned short )*out->_face_count * 3 );
			break;
		}
		int img_count = 3;
		stream->read( ( char* )&img_count , sizeof( int ) );
		if( img_count < 1 ) return out;
		out->_flags |= ShaderMask::MASK_TEXTURED | ShaderMask::MASK_TEXTURED_DIF | ShaderMask::MASK_TEXTURED_NOR;
		std::unique_ptr< RImage[] > img( new RImage[img_count] );
		ito( img_count )
		{
			loadImageBin( img[i] , stream );
		}
		out->_textures = std::move( img );
		out->_texture_count = img_count;
		return std::move( out );
    }
	static uint binarize( uint c )
	{
		uint t;
		ito( 32 )
		{
			t = 1 << i;
			if( t > c ) return t;
		}
		return 0;
	}
	static std::unique_ptr< RAnimationset[] > loadAnimSetBin( std::shared_ptr< std::ifstream > stream )
    {
		std::unique_ptr< RAnimationset[] > out( new RAnimationset[1] );
		/*int anim_count = 0;
		stream->read( ( char* )&anim_count , sizeof( int ) );
		int bcount = 0;
		stream->read( ( char* )&bcount , sizeof( int ) );
		int *skelet = new int[bcount*2];
		stream->read( ( char* )skelet , bcount * 2 * sizeof( int ) );/////*/
		int temp;
		stream->read( ( char * )&temp, sizeof( uint ) );
		stream->read( ( char * )&out[0]._bone_count, sizeof( uint ) );
		stream->read( ( char * )&out[0]._frame_count, sizeof( uint ) );
		///ATWECHAU PEREPISATI BLYAAAATY
		out[0].__data = std::move( std::unique_ptr< f4x4[] >( new f4x4[out[0]._frame_count*out[0]._bone_count] ) );
		stream->read( ( char * )out[0].__data.get() , sizeof( f4x4 )*out[0]._frame_count*out[0]._bone_count );
		return out;
    }
	/*static void loadScene( const char *filename , std::shared_ptr< RSceneDsc > out_scene )
	{
		try
		{
			RDrawableState &box = out_scene->getInstanceState( out_scene->addInstance(
																	out_scene->genDrawable( RDrawableType::RDRAWBL_BOX ) ) );
			box.getModelMat().scale( 10.0f );
			box.getModelMat()( 3 , 2 ) = 30.0f;
			RDrawableState &quad = out_scene->getInstanceState( out_scene->addInstance(
																	 out_scene->genDrawable( RDrawableType::RDRAWBL_QUAD ) ) );
			quad.getModelMat().scale( 100.0f );
			//MARK

			std::shared_ptr< std::ifstream > orc_s = getStream( "res/view/polymodels/monkey.bin" );
			RDrawablePTR orc_ptr = out_scene->genPolyMesh( loadPolyMeshBin( orc_s , RPolymesh::RPolyMeshType::RBONED_PMESH ) );
			std::unique_ptr< RAnimationset[] > orc_sets{ std::move( loadAnimSetBin( orc_s ) ) };
			out_scene->genBoneAnimInTex( std::move( orc_sets ) , 1 );
			int l = 10;
			ifor( i , 0 , l )
					ifor( j , 0 , l )
			{
				RDrawableState &state = out_scene->getInstanceState( out_scene->addInstance( orc_ptr ) );
				state.getModelMat()( 3 , 0 ) = 3.0f * ( i - l / 2 );
				state.getModelMat()( 3 , 1 ) = 3.0f * ( j - l / 2 );
			}
			out_scene->addLightSource( RLightState( true , RLightSourceType::RLIGHT_OMNI ,
													 RPassDesc( 512 , 512 , 1 , -1 , RBufferType::RBUFFER_FLOAT ) , f4( 1.0f , 1.0f , 1.0f , 1.0f ) ) );
			out_scene->addLightSource( RLightState( true , RLightSourceType::RLIGHT_DIRECT ,
													RPassDesc( 2048 , 2048 , 1 , -1 , RBufferType::RBUFFER_FLOAT ) , f4( 1.0f , 1.0f , 1.1f , 1.0f ) ) );
		}catch( std::logic_error const &err )
		{
			printf( err.what() );
		}
	}*/
	static std::shared_ptr< std::ifstream > getStream( const char *filename )
    {
		std::shared_ptr< std::ifstream > out( new std::ifstream( filename , std::ios::in | std::ios::binary ) );
		if( !out->is_open() )
			throw std::logic_error( "cannot open file\n" );
		return out;
    }
	static std::string loadFile( std::string filename )
    {
        std::ifstream file( filename );
        if( file.is_open() )
		{
			std::stringstream  out;
			std::string temp( "" );
			while( std::getline( file , temp ) )
			{
				out << temp << "\n";
			}
			out << "\0";
			file.close();
#ifdef RLOG
			std::cout << filename << " is loaded with " << out.str().size() << " char\n";
#endif
			return out.str();
		}
            else
        {
#ifdef RLOGERROR
			std::cout << filename << " is not loaded\n";
#endif
			throw std::logic_error( filename );
        }
    }
	static std::string append( std::string const &t1 , std::string const &t2 )
	{
		char *temp = new char( t1.length() + t2.length() );
		memcpy( temp , t1.c_str() , t1.length() );
		memcpy( temp + t1.length() , t2.c_str() , t2.length() );
		return std::string( temp );
	}
};
#endif //__RFileloader_H__
