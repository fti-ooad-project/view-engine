#version 430
layout( location = 0 ) in vec3 vertex_position;
layout( location = 1 ) in vec2 vertex_texcoord;
out vec2 frag_pos;
void main()
{
	frag_pos = vec2( vertex_texcoord.x , 1.0 - vertex_texcoord.y );
	gl_Position = vec4( vertex_position , 1.0 );
}
