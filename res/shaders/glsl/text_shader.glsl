#version 430
layout( location = 0 ) uniform sampler2D TEXT_BUFFER;
in vec2 frag_pos;
out vec4 out_data;
void main()
{
	vec4 data = texture2D( TEXT_BUFFER , vec2( frag_pos.x , 1.0 - frag_pos.y ) );
	out_data = vec4( pow( data.x , 2.0 ) );
}
