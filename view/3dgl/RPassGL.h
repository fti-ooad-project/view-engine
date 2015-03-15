#ifndef RPASSGL_H
#define RPASSGL_H
#include "RPassDesc.h"
class RDrawPassGL : public RInitable
{
private:
	RPassDesc _desc;
	uint _depth_buffer_pointer = 0;
	uint _framebuffer_id = 0;
	uint N;
	std::unique_ptr< uint[] > __texture_pointer_array;
public:
	RDrawPassGL() = default;
	void init(const RPassDesc &);
	ViewTypes::RDrawBufferPTR getBufferPtr(int ) const;
	uint getBufferCount();
	void clear(bool cd = true);
	void bind() const;
	ViewTypes::RDrawBufferPTR getDepthBufferPtr() const;
	void release() override;
	void update(const RSize &);
};
#endif // RPASSGL_H
