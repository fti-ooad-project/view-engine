#version 430
layout( location = 0 ) uniform sampler2D TEXTURE;
in vec2 tex_coord;
out vec4 buf;

void main()
{
	buf = texture2D( TEXTURE , tex_coord );
}  