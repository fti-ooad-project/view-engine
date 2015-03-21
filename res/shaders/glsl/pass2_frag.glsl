#version 430
$include include.glsl
$include light_func.glsl
layout( location = 0 ) uniform sampler2D BUFFER0;
layout( location = 1 ) uniform usampler2D BUFFER1;
layout( location = 2 ) uniform usampler2D WATER_BUFFER3;
layout( location = 3 ) uniform Camera CAM;

in vec2 frag_pos;
out vec3 out_data;
void main()
{
	out_data = texture( BUFFER0 , frag_pos ).xyz;
}