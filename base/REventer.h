#ifndef REVENTER_H
#define REVENTER_H
#include "RBase.h"
#include <functional>
typedef bool keystate;
struct	KeyStates
{
	const keystate * __cur_states;
	const keystate * __last_states;
	KeyStates(const keystate *, const keystate *);
};
struct	MouseStates
{
	const keystate *__cur_states;
	const keystate *__last_states;
	const f2 __cur_pos;
	const f2 __last_pos;
	MouseStates(const keystate *, const keystate *, const f2 &, const f2 &);
};
typedef std::function< void(const KeyStates&, const float) > KeyFunc;
typedef std::function< void(const MouseStates&, const float) > MouseFunc;
typedef std::function< void(const float) > TimeFunc;
class REventer : public RInitable, public RTimer
{
private:
	std::vector< KeyFunc > _key_func;
	std::vector< MouseFunc > _mouse_func;
	std::vector< TimeFunc > _time_func;
	keystate    __key_state[2][MAX_KEY];
	keystate	__mouse_state[2][3];
	f2			__mouse_pos[2];
	int         _cur[3], _last[3];
public:
	REventer();
	void addMouseFunc(MouseFunc);
	void addKeyFunc(KeyFunc);
	void addTimeFunc(TimeFunc);
	void update(const keystate *, const keystate *, const f2 *);
	void call();
	void release();
	~REventer();
};
#endif // REVENTER_H
