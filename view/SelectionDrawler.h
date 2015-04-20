#pragma once
#include "RPassGL.h"
#include "RViewModelsGL.h"
#include "RDrawableGL.h"
#include "../../base/RBase.h"
#include "Blurer.h"
class SelectionDrawler : public RInitable
{
private:
	RDrawPassGL _process_pass;
	RGraphicProgrammGL _process_prog;
	RPolyQuadGL _screen_quad;
	Blurer _blurer;
	SelectionDrawler();
public:
	uint process( uint );
	static SelectionDrawler *getSingleton();
	void init() override;
	void release() override;
};
