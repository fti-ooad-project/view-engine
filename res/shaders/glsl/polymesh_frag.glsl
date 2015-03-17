#version 430
$include uniforms.glsl
$include include.glsl
$include pervertex.glsl
in PerVertex pvo;

out uvec4 buf;

void main()
{
	if( bool( LIGHTPASS ) )
	{
		return;
	}
	vec4 diff = vec4( 0.5 );
	vec4 norm = vec4( pvo.normal , pvo.depth );
	vec4 spec = vec4( vec3( 0.2 ) , 0.2 );
	if( bool( FLAGS & MASK_TEXTURED ) )
	{
		if( bool( FLAGS & MASK_TEXTURED_DIF ) )
			diff = texture2D( RGB_DIFFUSE_A_OPACITY_TEXTURE , pvo.texcoord );
		//if( diff.w < 0.5 ) discard;
		if( bool( FLAGS & MASK_TEXTURED_NOR ) && pvo.dist_to_cam < 10.0 )
		{
			vec4 ne = texture2D( RGB_NORMAL_A_HEIGHT_TEXTURE , pvo.texcoord );
			vec4 bump = -1.0 + ne * 2.0;
			vec3 newnormal = normalize( pvo.normal * bump.z-pvo.binormal * bump.y + pvo.tangentnormal * bump.x );
			norm = vec4( newnormal.xyz , pvo.depth);
		}
		if( bool( FLAGS & MASK_TEXTURED_SPE ) )
			spec= vec4( texture2D( RGB_SPECULAR_A_GLOSSNESS_TEXTURE , pvo.texcoord ) );
		//buf[3] = vec4( difo.xyz , ne.w );
	}
	buf.x = uint( pvo.depth * 100.0 );
	buf.y = pack4i( vec4( diff.xyz , pvo.select_id ) );
	float sg = norm.z >= 0.0 ? 1.0 : -1.0;
	buf.z = pack4i( vec4( 0.5 + 0.5 * norm.xy , 0.5 + 0.5 * sg , 0.0 ) );
	buf.w = pack4i( spec );
	//buf[1] = vec4( pvo.normal , pvo.depth );
	//buf[2] = vec4( 0.0 );
	//buf[3] = vec4( vec3( 1.0 ) , 0.0 );
	//buf[0].x = pvo.depth;
}  
