#version 430
$include uniforms.glsl
$include anim_include.glsl
$include include.glsl
$include attributes.glsl
$include pervertex.glsl
out PerVertex pv;

void main()
{
	vec4 p = vec4( invertex_position , 1.0 );
	mat4 m;
	//if( bool( INSTANSING ) )
	//{
	
	pv.dist_to_cam = INS_TIME.z;
	float vz = INS_POS.z;
	if( bool( INS_MIXING_ANIM_INDX.w ) )
	{
		float h = texture( BUFFER1 , 0.5 + 0.5 * INS_POS.xy / HEIGHTMAP_SIZE.xy , 0.0 ).x;
		vz = ( h - 0.5 ) * HEIGHTMAP_SIZE.z;
	}
	m = mat4(
		vec4( INS_LEFT.x , INS_LOOK.x , INS_UP.x , 0.0 ) ,
		vec4( INS_LEFT.y , INS_LOOK.y , INS_UP.y , 0.0 ) ,
		vec4( INS_LEFT.z , INS_LOOK.z , INS_UP.z , 0.0 ) ,
		vec4( INS_POS.xy , vz , 1.0 )
	);
	//}
	
	$include anim_func.glsl
	
	p = m * p;
	gl_Position = p;
	switch( PASSID )
	{
		case PASS_LIGHT:
		break;
		case PASS_WATER:
		break;
		case PASS_NORMAL:
			if( bool( FLAGS & MASK_TEXTURED_NOR ) )
			{
				pv.binormal = normalize( ( m * vec4( invertex_binormal , 0.0 ) ).xyz );
				pv.tangentnormal = normalize( ( m * vec4( invertex_tangentnormal , 0.0 ) ).xyz );
			}
			if( bool( FLAGS & MASK_TEXTURED ) )
				pv.texcoord = invertex_texcoord;
			pv.normal = normalize( ( m * vec4( invertex_normal , 0.0 ) ).xyz );
			pv.select_id = INS_TIME.w;
		break;
		default:
		break;
	}
} 
