#version 430
layout( location = 0 ) in vec3 vertex_position;
layout( location = 1 ) in vec2 vertex_texcoord;
layout( location = 5 ) uniform vec4 POS_SIZE;
out vec2 frag_pos;
void main()
{
	frag_pos = vertex_texcoord;
	vec4 p;
	p = vec4( POS_SIZE.xy + vertex_position.xy * POS_SIZE.zw , 0.0 , 1.0 );
	gl_Position = p;
}
