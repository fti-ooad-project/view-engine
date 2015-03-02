#ifndef RANIMGL_H
#define RANIMGL_H
#include "RAnim.h"
#include "../GL.h"
class RBoneAnimInTexHolderGL
{
public:
	std::unique_ptr< RAnimationset[] > __sets;
public:
	int _count;
	int _bone_count;
	std::unique_ptr< uint[] > __texture_pointer_array;
	std::unique_ptr< int[] > __frame_count;
	RBoneAnimInTexHolderGL( std::unique_ptr< RAnimationset[] > &&sets , int count ):
	_count( count )
	, __sets( std::move( sets ) )
	, __frame_count( new int[count] )
	, _bone_count( __sets.get()[0]._bone_count )
	{
	}
	void operator=( RBoneAnimInTexHolderGL &&a )
	{
		_count =  a._count;
		if( !_count ) return;
		__sets = std::move( a.__sets );
		__frame_count = std::move( std::unique_ptr< int[] >( new int[_count] ) );
		_bone_count = __sets[0]._bone_count;
	}
	void init()
	{
		__texture_pointer_array = std::move( std::unique_ptr< uint[] >( new uint[_count] ) );
		glGenTextures( _count , __texture_pointer_array.get() );
		ito( _count )
		{
			__frame_count[i] = __sets[i]._frame_count;
			glBindTexture( GL_TEXTURE_2D , __texture_pointer_array[i] );
			glTexImage2D( GL_TEXTURE_2D , 0 , GL_RGBA32F , _bone_count * 4 , __frame_count[i] , 0 ,
					GL_RGBA , GL_FLOAT , __sets[i].__data.get() );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MAG_FILTER , GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_MIN_FILTER , GL_LINEAR );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_S , GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D , GL_TEXTURE_WRAP_T , GL_REPEAT );
		}
		this->__sets.reset();
	}
	void release()
	{
		glDeleteTextures( _count , __texture_pointer_array.get() );
		__frame_count.reset();
		__texture_pointer_array.reset();
	}
	ViewTypes::RDrawBufferPTR getBufferPtr( int i ) const
	{
		return __texture_pointer_array[i];
	}
};
#endif // RANIMGL_H
