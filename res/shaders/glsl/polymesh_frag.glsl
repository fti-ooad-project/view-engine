#version 430

$include uniforms.glsl
$include include.glsl
$include pervertex.glsl
in PerVertex pvo;

out uvec4 buf;
//#extension GL_EXT_texture_aray : enable
//#extension GL_NV_texture_aray : enable
void main()
{
	switch( PASSID )
	{
		case PASS_REFLECT:
			if( pvo.position.z < WATER_POS_SIZE.z ) discard;
		break;
		case PASS_WATER:
		case PASS_LIGHT:
			buf = uvec4( 1 );
		break;
		case PASS_NORMAL:
			vec4 diff = vec4( 0.5 );
			vec4 norm = vec4( pvo.normal , pvo.depth );
			vec4 spec = vec4( vec3( 0.2 ) , 0.2 );
			if( bool( FLAGS & MASK_TEXTURED ) )
			{
				if( bool( FLAGS & MASK_TEXTURED_DIF ) )
					diff = texture( TEXTURE_ARRAY , vec3( pvo.texcoord , 1.0 ) );
				//if( diff.w < 0.5 ) discard;
				if( bool( FLAGS & MASK_TEXTURED_NOR ) && pvo.dist_to_cam < 10.0 )
				{
					vec4 ne = texture( TEXTURE_ARRAY , vec3( pvo.texcoord , 0.0 ) );
					vec4 bump = -1.0 + ne * 2.0;
					vec3 newnormal = normalize( pvo.normal * bump.z-pvo.binormal * bump.y + pvo.tangentnormal * bump.x );
					norm = vec4( newnormal.xyz , pvo.depth);
				}
				if( bool( FLAGS & MASK_TEXTURED_SPE ) )
					spec = texture( TEXTURE_ARRAY , vec3( pvo.texcoord , 2.0 ) );
				//buf[3] = vec4( difo.xyz , ne.w );
			}
			buf.x = uint( pvo.depth * 100.0 );
			buf.y = pack4i( 
			//vec4( 0.5 + 0.5 * pvo.normal , 0.0 ));//
			vec4( diff.xyz , pvo.select_id ) );
			float sg = norm.z >= 0.0 ? 1.0 : -1.0;
			buf.z = pack4i( vec4( 0.5 + 0.5 * norm.xy , 0.5 + 0.5 * sg , 0.0 ) );
			buf.w = pack4i( spec );
			//buf[1] = vec4( pvo.normal , pvo.depth );
			//buf[2] = vec4( 0.0 );
			//buf[3] = vec4( vec3( 1.0 ) , 0.0 );
			//buf[0].x = pvo.depth;
		break;
		default:
		break;
	}
}  
