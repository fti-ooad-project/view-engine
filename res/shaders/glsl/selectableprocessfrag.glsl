#version 430
layout( location = 0 ) uniform usampler2D BUFFER;
layout( location = 1 ) uniform sampler2D BUFFER1;
layout( location = 2 ) uniform int pass;
$include include.glsl
in vec2 frag_pos;
out vec4 out_data;
void main()
{
	out_data = vec4( 0.0 );
	if( pass == 0 )
	{
		uvec4 buf = texture( BUFFER , frag_pos );
		int id = int( unpack4i( buf.y ).w * 10.0 );
		out_data = vec4( 0.0 , 0.0 , 0.0 , 1.0 );
		//if( id == 1 )
		//	out_data = vec4( 1.0 , 0.0 , 0.0 , 1.0 );
		//else
		if( id == 2 )
			out_data = vec4( 0.0 , 1.0 , 0.0 , 1.0 );
		return;
	}
	if( pass == 1 )
	{
		uvec4 buf = texture( BUFFER , frag_pos );
		float id = unpack4i( buf.y ).w;
		if( id > 0.01 )
			out_data = vec4( 0.0 );
		else
			out_data = texture( BUFFER1 , frag_pos );
		return;
	}
}