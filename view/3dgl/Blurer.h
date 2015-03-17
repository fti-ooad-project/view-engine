#pragma once
#include "RPassGL.h"
#include "RViewModelsGL.h"
#include "RDrawableGL.h"
#include "../../base/RBase.h"
class Blurer : public RInitable
{
private:
	RDrawPassGL _process_pass;
	RGraphicProgrammGL _process_prog;
	RPolyQuadGL _screen_quad;
public:
	Blurer() = default;
	uint process( uint , float );
	//static Blurer *getSingleton();
	void init() override;
	void release() override;
};
