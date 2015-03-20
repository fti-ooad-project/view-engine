#ifndef __RPASSDESC_H__
#define __RPASSDESC_H__
#include "../../base/RBase.h"
#include "../export/RViewModels.h"
class RBufferStoreType
{
public:
	static const int RBUFFER_BYTE = 0 , RBUFFER_FLOAT = 1 , RBUFFER_INT = 2;
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
	int _component_number;
};
#endif /* __RPASSDESC_H__ */
