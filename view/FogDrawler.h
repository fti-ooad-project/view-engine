#pragma once
#include <base/Base.h>
#include <openGL/PassGL.h>
#include <openGL/ViewModelsGL.h>
#include <view/ViewModels.h>
#include <view/Camera.h>
class FogDrawler : public Initable
{
private:
	FogDrawler() = default;
	GraphicProgrammGL _prog;
	DrawPassGL _pass;
public:
	static FogDrawler *getSIngleton();
	void init( uint , uint );
	void release() override;
	DrawPassGL const &process( uint , uint , DrawPassGL const * , f4x4 const * , Camera const * );
};
