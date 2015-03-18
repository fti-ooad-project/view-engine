#version 430
$include include.glsl
$include uniforms.glsl
$include attributes.glsl
$include pervertex.glsl
out PerVertex pvte;

void main()
{
	vec4 p = vec4( invertex_position , 1.0 );
	mat4 m;
	if( bool( INSTANSING ) )
	{
		pvte.dist_to_cam = INS_TIME.z;
		m = INS_MODEL4x4;
	}
	else
	{
		m = MAT4X4_MODEL;
		pvte.dist_to_cam = DIST_TO_CAM;
	}

	$include anim_func.glsl

	p = m * p;
	vec4 pos =vec4( p.xyz , 1.0 );
	switch( PASSID )
	{
		case PASS_LIGHT:
			gl_Position = scalel( pos );
		break;
		case PASS_WATER:
			gl_Position = pos;
		case PASS_NORMAL:
			gl_Position = pos;
			if( bool( FLAGS & MASK_TEXTURED_NOR ) )
			{
				pvte.binormal = normalize( ( m * vec4( invertex_binormal , 0.0 ) ).xyz );
				pvte.tangentnormal = normalize( ( m * vec4( invertex_tangentnormal , 0.0 ) ).xyz );
			}
			if( bool( FLAGS & MASK_TEXTURED ) )
				pvte.texcoord = invertex_texcoord;
			pvte.normal = normalize( ( m * vec4( invertex_normal, 0.0 ) ).xyz );
			pvte.select_id = INS_TIME.w;
			pvte.position = p.xyz;
			pvte.depth = pos.z;
		break;
		default:
		break;
	}
}
