#ifndef __RPASSDESC_H__
#define __RPASSDESC_H__
#include "base/RBase.h"
#include "view/3dgl/RViewModels.h"
class RBufferStoreType
{
	static const int RBUFFER_BYTE = 0 , RBUFFER_FLOAT = 1;
};
struct RRect
{
	uint _x , _y , _w , _h;
};
struct RPassDesc
{
	RSize _size;
	int _store_type;
	uint _buffer_count;
	int _foreight_depth_buffer;
	bool _depth_buffer_access;
	bool _cubemap;
};
class RPass : public RInitable
{
public:
	virtual void init( const RPassDesc & ) = 0;
	virtual void update( const RSize & ) = 0;
	virtual void clear() = 0;
	virtual void bind() const = 0;
	virtual ViewTypes::RDrawBufferPTR getBufferPtr( int ) const = 0;
	virtual ViewTypes::RDrawBufferPTR getDepthBufferPtr() const = 0;
	virtual uint getBufferCount() = 0;
};
#endif /* __RPASSDESC_H__ */
