#version 430
$include include.glsl
$include attributes.glsl
$include uniforms.glsl

void main()
{
	vec4 p = vec4( invertex_position , 1.0 );
	mat4 m = MAT4X4_MODEL;
	$include anim_func.glsl
	p = m * p;
	gl_Position = vec4( p.xyz , 1.0 );
}