#include "../view/GL.h"
#include "../view/3dgl/RAnimGL.h"
uint RBoneAnimInTexHolderGL::getCount() const
{
	return _count;
}
uint RBoneAnimInTexHolderGL::getTexture(uint i) const
{
	return __texture_pointer_array[i];
}
void RBoneAnimInTexHolderGL::operator=(RBoneAnimInTexHolderGL &&a)
{
	release();
	_count = a._count;
	if (!_count) return;
	setInited(a.isInited());
	if (a.isInited())
	{
		__texture_pointer_array = std::move(a.__texture_pointer_array);
	}
	else
	{
		__sets = std::move(a.__sets);
		__frame_count = std::move(std::unique_ptr< int[] >(new int[_count]));
		_bone_count = __sets[0]._bone_count;
		__texture_pointer_array = std::move(std::unique_ptr< uint[] >(new uint[_count]));
	}
	a.release();
}
RBoneAnimInTexHolderGL::RBoneAnimInTexHolderGL(RBoneAnimInTexHolderGL &&a)
{
	*this = std::move(a);
}
RBoneAnimInTexHolderGL::RBoneAnimInTexHolderGL(std::unique_ptr< RAnimationset[] > &&sets, int count)
{
	_count = count;
	__sets = std::move(sets);
	__frame_count = std::move(std::unique_ptr< int[] >(new int[_count]));
	_bone_count = __sets[0]._bone_count;
	__texture_pointer_array = std::move(std::unique_ptr< uint[] >(new uint[_count]));
}
void RBoneAnimInTexHolderGL::init(std::unique_ptr< RAnimationset[] > &&sets, int count)
{
	_count = count;
	__sets = std::move(sets);
	__frame_count = std::move(std::unique_ptr< int[] >(new int[_count]));
	_bone_count = __sets[0]._bone_count;
	__texture_pointer_array = std::move(std::unique_ptr< uint[] >(new uint[_count]));
	init();
}
void RBoneAnimInTexHolderGL::init()
{
	if (isInited() || _count == 0) return;
	setInited(true);
	glGenTextures(_count, __texture_pointer_array.get());
	ito(_count)
	{
		__frame_count[i] = __sets[i]._frame_count;
		glBindTexture(GL_TEXTURE_2D, __texture_pointer_array[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _bone_count * 4, __frame_count[i], 0,
			GL_RGBA, GL_FLOAT, __sets[i].__data.get());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	this->__sets.reset();
}
void RBoneAnimInTexHolderGL::release()
{
	if (!isInited()) return;
	setInited(false);
	glDeleteTextures(_count, __texture_pointer_array.get());
	__frame_count.reset();
	__texture_pointer_array.reset();
}
ViewTypes::RDrawBufferPTR RBoneAnimInTexHolderGL::getBufferPtr(int i) const
{
	return __texture_pointer_array[i];
}