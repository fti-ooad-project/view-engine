#version 430
layout( location = 0 ) uniform sampler2D FIELD_BUFFER;
layout( location = 1 ) uniform sampler2D COLOR_BUFFER;
layout( location = 2 ) uniform float LEVEL;
in vec2 frag_pos;
out vec4 out_data;
void main()
{
	vec4 data = texture( FIELD_BUFFER , frag_pos );
	vec4 color = texture( COLOR_BUFFER , vec2( data.x , LEVEL / 32.0 ) );
	out_data = color;//vec4( vec3( 0.5 , 0.5 , 0.5 ) , 1.0 - data.x );//vec4( pow( 1.0 - data.x , 1.0 ) );//vec4( frag_pos , 0.0 , 1.0 );
}