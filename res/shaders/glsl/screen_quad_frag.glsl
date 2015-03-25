#version 430
layout( location = 0 ) uniform sampler2D BUFFER;
//layout( location = 1 ) uniform sampler2D SMTH_BUFFER;//to average brightness and scale light range
in vec2 frag_pos;
out vec4 out_data;
vec4 hdr( vec4 v , float s , float e )
{
	const float ds = e - s;
	return pow( ( v - s ) / ds , vec4( 1.2 ) );
}
void main()
{
	//vec4 c = texture2D( SMTH_BUFFER , vec2( 0.5 , 0.5 ) );
	float bright = 0.5;//abs( c.x + c.y + c.z ) / 3;
	out_data = hdr( texture2D( BUFFER , vec2( frag_pos.x , frag_pos.y ) ) , max( 0.0 , bright - 0.4 ) , bright + 0.4 );
}
