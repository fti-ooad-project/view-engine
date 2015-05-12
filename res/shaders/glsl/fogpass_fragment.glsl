#version 430
$include include.glsl
$include light_func.glsl
layout( location = 0 ) uniform int LCOUNT;
layout( location = 1 ) uniform usampler2D BUFFER0;
layout( location = 2 ) uniform sampler2D LIGHTDEPTH[ 10 ];
layout( location = 12 ) uniform Camera CAM;
layout( location = 20 ) uniform mat4 LIGHTVIEWROJ[ 10 ];
out vec3 out_color;
in vec2 frag_pos;
void main()
{
	uvec4 buf = texture( BUFFER0 , frag_pos );
	float depth = depthFromi( buf );
	if( buf.x == 0 )
	{
		depth = 50.0;
	}
	vec3 v = camRay( CAM , frag_pos );
	const int SAMPLES_COUNT = 20;
	float c = vec3( 0.0 );
	for( int i = 0; i < SAMPLES_COUNT; i ++ )
	{
		float dist = float( i + 1 ) / SAMPLES_COUNT * min( depth , 50.0 );
		vec3 tv = CAM.pos + v * dist;
		vec4 p = LIGHTVIEWROJ[ 0 ] * vec4( tv , 1.0 );
		c += lightedDir( p , LIGHTDEPTH[ 0 ] );
	}
	out_color = vec3( pow( c / SAMPLES_COUNT , 1.0 ) );
}