#ifndef __RBuffers_H__
#define __RBuffers_H__
//#include <conio.h>
#include "../linalg/vec.h"
struct RVertex
{
	f3			Position;
	f3			Normal;
	f3			BiNormal;
	f3			TangentNormal;
	f2			TextureCoordinate;
	f3			BoneWeights;
	int         BoneIndex[4];
};
struct RStaticVertex
{
	f3			Position;
	f3			Normal;
	f3			BiNormal;
	f3			TangentNormal;
	f2			TextureCoordinate;
};
struct RVertexOffsets
{
	static const int			Position=0;
	static const int			Normal=sizeof(f3);
	static const int			BiNormal=sizeof(f3)*2;
	static const int			TangentNormal=sizeof(f3)*3;
	static const int			TextureCoordinate=sizeof(f3)*4;
	static const int			BoneWeights=sizeof(f2)+sizeof(f3)*4;
	static const int            BoneIndex=sizeof(f2)+sizeof(f3)*5;
    static const int            Base=sizeof(RVertex);
};
struct RStaticVertexOffsets
{
	static const int			Position=0;
	static const int			Normal=sizeof(f3);
	static const int			BiNormal=sizeof(f3)*2;
	static const int			TangentNormal=sizeof(f3)*3;
	static const int			TextureCoordinate=sizeof(f3)*4;
	static const int            Base=sizeof(RStaticVertex);
};
#endif /* __RBuffers_H__ */
