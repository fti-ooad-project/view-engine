#include "../base/RBase.h"
RSize::RSize(uint w , uint h) : _w(w), _h(h) {}
RSize::RSize(const RSize &size) : _w(size._w), _h(size._h) {}
void RTimer::updateTime()
{
	_last_time = _cur_time;
	//std::time_t temp = std::time( nullptr );
	clock_t time = clock();
	_cur_time = 0.001f * static_cast<float>(time * TIMERK);//QTime::currentTime().msecsSinceStartOfDay();
	_dt = fabs(_cur_time - _last_time);
}
RInitable::RInitable()
{
#if RDEBUG
	//        printf( "__initable added in super %i\n" , ++_c );
#endif
}
void RInitable::setInited(bool i)
{
	_inited = i;
}
bool RInitable::isInited() const
{
	return _inited;
}
void RInitable::init() {}
RInitable::~RInitable()
{
#if RDEBUG
	//        printf( "__initable deleted in super %i\n" , --_c );
#endif
}