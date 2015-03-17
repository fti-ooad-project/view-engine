#version 430
$include uniforms.glsl
$include include.glsl
$include attributes.glsl
$include pervertex.glsl
out PerVertex pv;

void main()
{
	vec4 p = vec4( invertex_position , 1.0 );
	mat4 m;
	if( bool( INSTANSING ) )
	{
		pv.dist_to_cam = INS_TIME.z;
		m = INS_MODEL4x4;
	}
	else
	{
		m = MAT4X4_MODEL;
		pv.dist_to_cam = DIST_TO_CAM;
	}
		
	$include anim_func.glsl
	
	p = m * p;	
	if( bool( FLAGS & MASK_TEXTURED_NOR ) )
	{
		pv.binormal = normalize( ( m * vec4( invertex_binormal , 0.0 ) ).xyz );
		pv.tangentnormal = normalize( ( m * vec4( invertex_tangentnormal , 0.0 ) ).xyz );
	}
	if( bool( FLAGS & MASK_TEXTURED ) )
		pv.texcoord = invertex_texcoord;
	pv.normal = normalize( ( m * vec4( invertex_normal , 0.0 ) ).xyz );
	pv.select_id = INS_TIME.w;
    gl_Position = p;
} 
