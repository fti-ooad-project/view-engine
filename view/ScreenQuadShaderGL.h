#pragma once
#include <openGL\ShaderBuilderGL.h>
class ScreenQuadShaderGL : ShaderBaseGL
{
private:
public:
	ShaderType _screen_texture;
	void init() override;
};