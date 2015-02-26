#version 430
$include uniforms.glsl
$include include.glsl
$include attributes.glsl
$include pervertex.glsl
out PerVertex pv;

void main()
{
	vec4 p = vec4( invertex_position , 1.0 );
	mat4 m = MAT4X4_MODEL;
	$include anim_func.glsl
	p = m * p;	
	if( bool( FLAGS & MASK_TEXTURED_NOR ) )
	{
		pv.binormal = normalize( ( m * vec4( invertex_binormal , 0.0 ) ).xyz );
		pv.tangentnormal = normalize( ( m * vec4( invertex_tangentnormal , 0.0 ) ).xyz );
	}
	if( bool( FLAGS & MASK_TEXTURED ) )
		pv.texcoord = invertex_texcoord;
	pv.normal = normalize( ( m * vec4( invertex_normal, 0.0 ) ).xyz );
    gl_Position = p;
} 
