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
	uvec4 wbuf = texture( WATER_BUFFER3 , frag_pos );
	vec4 wY = unpack4i( wbuf.y );
	if( wY.w < 0.1 ) discard;
	float start_depth = depthFromi( wbuf );
	vec3 wnorm = normFromi( wbuf );
	const int COUNT = 10;
	const float DIST = 5.0;
	vec3 wpos = posFromZ( vec2( -1.0 + 2.0 * frag_pos.x , -1.0 + 2.0 * frag_pos.y ) , wdepth , CAM );
	float dest_depth = dot( wpos + DIST * reflect( normalize( wpos - CAM.pos ) , wnorm ) , CAM.look ) - CAM.pos * CAM.look;
	float DDEPTH = ( dest_depth - start_depth ) / COUNT;
	for( int i = 1; i < COUNT + 1; i++ )
	{
		float idepth = start_depth + i * DDEPTH;
		
	}
	out_data = texture( BUFFER0 , frag_pos ).xyz;
}