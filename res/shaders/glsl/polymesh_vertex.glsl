#version 430
$include include.glsl
$include uniforms.glsl
$include attributes.glsl
$include pervertex.glsl
out PerVertex pvo;

void main()
{
	vec4 p = vec4( invertex_position , 1.0 );
	mat4 m;
	if( bool( INSTANSING ) )
		m = INS_MODEL4x4;
	else
		m = MAT4X4_MODEL;
	
	$include anim_func.glsl
	
	p = m * p;
	if( bool( FLAGS & MASK_TEXTURED_NOR ) )
	{
		pvo.binormal = normalize( ( m * vec4( invertex_binormal , 0.0 ) ).xyz );
		pvo.tangentnormal = normalize( ( m * vec4( invertex_tangentnormal , 0.0 ) ).xyz );
	}
	if( bool( FLAGS & MASK_TEXTURED ) )
		pvo.texcoord = invertex_texcoord;
	pvo.normal = normalize( ( m * vec4( invertex_normal, 0.0 ) ).xyz );
    vec4 pos = MAT4X4_VIEWPROJ[0] * vec4( p.xyz , 1.0 );
	pvo.position = p.xyz;
	pvo.depth = pos.z;
    gl_Position = pos;
}
