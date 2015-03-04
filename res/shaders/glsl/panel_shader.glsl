#version 430
layout( location = 0 ) uniform sampler2D FIELD_BUFFER;
layout( location = 1 ) uniform sampler2D COLOR_BUFFER;
in vec2 frag_pos;
out vec4 out_data;
void main()
{
	vec4 data = texture2D( FIELD_BUFFER , frag_pos );
	out_data = vec4( texture2D( COLOR_BUFFER , vec2( data.x , 0.0 ) ).xyz , 1.0 );
}
