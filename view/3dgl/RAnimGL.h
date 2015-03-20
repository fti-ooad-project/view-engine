#ifndef RANIMGL_H
#define RANIMGL_H
#include "RAnim.h"
#include "../export/RViewModels.h"
class RBoneAnimInTexHolderGL : public RInitable
{
public:
	std::unique_ptr< RAnimationset[] > __sets;
	int _count = 0;
	int _bone_count;
	//std::unique_ptr< uint[] > __texture_pointer_array;
	uint _texture_array_pointer;
public:
	RBoneAnimInTexHolderGL() = default;
	uint getCount() const;
	void operator=(RBoneAnimInTexHolderGL &&a);
	RBoneAnimInTexHolderGL(RBoneAnimInTexHolderGL &&a);
	RBoneAnimInTexHolderGL(std::unique_ptr< RAnimationset[] > &&sets, int count);
	void init(std::unique_ptr< RAnimationset[] > &&sets, int count);
	void init();
	void release();
	uint getBufferPtr() const;
};
#endif // RANIMGL_H
