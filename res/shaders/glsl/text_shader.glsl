#version 430
layout( location = 0 ) uniform sampler2D TEXT_BUFFER;
in vec2 frag_pos;
out vec4 out_data;
void main()
{
	vec4 data = texture2D( TEXT_BUFFER , frag_pos );
	//if( data.w < 0.1 ) discard;
	float c = data.w > 0.5 ? 0.0 : 1.0;
	out_data = vec4( vec3( c ) , 1.0 - c );
}
