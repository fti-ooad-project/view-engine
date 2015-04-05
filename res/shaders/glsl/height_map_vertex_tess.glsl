#version 430
$include uniforms.glsl
layout( location = 0 ) in vec3 invertex_position;
$include pervertex.glsl
out PerVertex pv;
void main()
{
	
	/*if( bool( FLAGS & MASK_TEXTURED_NOR ) )
	{
		pv.binormal = normalize( ( m * vec4( invertex_binormal , 0.0 ) ).xyz );
		pv.tangentnormal = normalize( ( m * vec4( invertex_tangentnormal , 0.0 ) ).xyz );
	}
	if( bool( FLAGS & MASK_TEXTURED ) )
		pv.texcoord = invertex_texcoord;
	pv.normal = normalize( ( m * vec4( invertex_normal , 0.0 ) ).xyz );*/
	float h = texture2D( BUFFER1 , 0.5 + 0.5 * invertex_position.xy ).x;
	vec4 p = vec4( HEIGHTMAP_SIZE.xy * invertex_position.xy , ( h - 0.5 ) * HEIGHTMAP_SIZE.z , 1.0 );
	pv.texcoord = 0.5 + 0.5 * invertex_position.xy;
	gl_Position = p;
	switch( PASSID )
	{
		case PASS_LIGHT:
		break;
		case PASS_WATER:
		break;
		case PASS_NORMAL:
		break;
		default:
		break;
	}
	//pvo.normal = vec3( 0.0 , 0.0 , 1.0 );
	//vec4 pos = MAT4X4_VIEWPROJ[0] * vec4( p.xyz , 1.0 );
	//pvo.depth = pos.z;
} 
