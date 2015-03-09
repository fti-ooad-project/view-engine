#version 430
$include uniforms.glsl
$include include.glsl
$include pervertex.glsl
in PerVertex pvo;

out vec4 buf[4];

void main()
{
	vec4 diff = vec4( 1.0 );
	vec4 norm = vec4( pvo.normal , pvo.depth );
	if( bool( FLAGS & MASK_TEXTURED ) )
	{
		if( bool( FLAGS & MASK_TEXTURED_DIF ) )
			diff = texture2D( RGB_DIFFUSE_A_OPACITY_TEXTURE , pvo.texcoord.xy );
		if( diff.w < 0.5 ) discard;
		if( bool( FLAGS & MASK_TEXTURED_NOR ) && pvo.dist_to_cam < 30.0 )
		{
			vec4 ne = texture2D( RGB_NORMAL_A_HEIGHT_TEXTURE , pvo.texcoord.xy );
			vec4 bump = -1.0 + ne * 2.0;
			vec3 newnormal = normalize( pvo.normal * bump.z-pvo.binormal * bump.y + pvo.tangentnormal * bump.x );
			norm = vec4( newnormal.xyz , pvo.depth);
		}
		if( bool( FLAGS & MASK_TEXTURED_SPE ) )
			buf[2] = vec4( texture2D( RGB_SPECULAR_A_GLOSSNESS_TEXTURE , pvo.texcoord.xy ) );
		//buf[3] = vec4( difo.xyz , ne.w );
	}
	buf[0].x = pvo.depth;
	buf[0].y = pack4f( vec4( diff.xyz , 0.0 ) );
	float sg = norm.z >= 0.0 ? 1.0 : -1.0;
	buf[0].z = pack4f( vec4( 0.5 + 0.5 * norm.xy , 0.5 + 0.5 * sg , 0.0 ) );
	//buf[1] = vec4( pvo.normal , pvo.depth );
	//buf[2] = vec4( 0.0 );
	//buf[3] = vec4( vec3( 1.0 ) , 0.0 );
	//buf[0].x = pvo.depth;
}  
