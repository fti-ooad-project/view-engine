#version 430
layout( location = 1 ) uniform mat4 MAT4X4_VIEWPROJ;
layout( location = 0 ) in vec3 invertex_position;
out vec2 tex_coord;

void main()
{
	tex_coord = invertex_position.xy;
    vec4 pos = MAT4X4_VIEWPROJ * vec4( invertex_position , 0.0 );
    gl_Position = vec4( pos.xyz / pos.z , 1.0 );
}
