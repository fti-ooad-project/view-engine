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
	std::unique_ptr< uint[] > __texture_pointer_array;
	std::unique_ptr< int[] > __frame_count;
public:
	RBoneAnimInTexHolderGL() = default;
	uint getCount() const;
	uint getTexture(uint i) const;
	void operator=(RBoneAnimInTexHolderGL &&a);
	RBoneAnimInTexHolderGL(RBoneAnimInTexHolderGL &&a);
	RBoneAnimInTexHolderGL(std::unique_ptr< RAnimationset[] > &&sets, int count);
	void init(std::unique_ptr< RAnimationset[] > &&sets, int count);
	void init();
	void release();
	ViewTypes::RDrawBufferPTR getBufferPtr(int i) const;
};
#endif // RANIMGL_H
