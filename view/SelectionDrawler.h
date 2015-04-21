#pragma once
#include <openGL\PassGL.h>
#include <openGL\DrawableGL.h>
#include <openGL\Blurer.h>
#include <base/Base.h>
#include <openGL\ViewModelsGL.h>
class SelectionDrawler : public Initable
{
private:
	DrawPassGL _process_pass;
	GraphicProgrammGL _process_prog;
	PolyQuadGL _screen_quad;
	Blurer _blurer;
	SelectionDrawler();
public:
	uint process( uint );
	static SelectionDrawler *getSingleton();
	void init() override;
	void release() override;
};
