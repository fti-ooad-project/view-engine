#version 430
layout( location = 0 ) in vec3 vertex_position;
layout( location = 1 ) in vec2 vertex_texcoord;
layout( location = 5 ) uniform vec4 POS_SIZE;
layout( location = 6 ) uniform vec4 TX_SIZE;
layout( location = 7 ) uniform float ANGLE;
out vec2 frag_pos;
void main()
{
	frag_pos = TX_SIZE.xy + vertex_texcoord * TX_SIZE.zw;
	vec4 p;
	if( ANGLE == 0.0 )
		p = vec4( POS_SIZE.xy + vertex_position.xy * POS_SIZE.zw , 0.0 , 1.0 );
	else
	{
		mat2 rot = mat2(
			cos( ANGLE ) , -sin( ANGLE ) ,
			sin( ANGLE ) , cos( ANGLE )
		);
		p = vec4( POS_SIZE.xy + ( vertex_position.xy * POS_SIZE.zw ) * rot , 0.0 , 1.0 );
	}
	gl_Position = p;
}
