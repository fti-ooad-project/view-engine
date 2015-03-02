#version 430
$include uniforms.glsl
$include include.glsl
$include pervertex.glsl
in PerVertex pvo;

out vec4 buf[4];

void main()
{
	buf[0] = vec4( pvo.position.xyz , 1.0);
	if( bool( FLAGS & MASK_TEXTURED ) )
	{
		vec4 difo = vec4( 0.0 );
		vec4 ne = vec4( 0.0 );
		//////////diff
		if( bool( FLAGS & MASK_TEXTURED_DIF ) )
			difo = texture2D( RGB_DIFFUSE_A_OPACITY_TEXTURE , pvo.texcoord.xy );
		if( difo.w < 0.5 ) discard;
		//////////norm
		if( bool( FLAGS & MASK_TEXTURED_NOR ) )
		{
			ne = texture2D( RGB_NORMAL_A_HEIGHT_TEXTURE , pvo.texcoord.xy );
			vec4 bump = -1.0 + ne * 2.0;
			vec3 newnormal = normalize( pvo.normal * bump.z-pvo.binormal * bump.y + pvo.tangentnormal * bump.x );
			buf[1] = vec4( newnormal.xyz , pvo.depth);
		}else
			buf[1] = vec4( pvo.normal , pvo.depth );
		//////////spec
		if( bool( FLAGS & MASK_TEXTURED_SPE ) )
			buf[2] = vec4( texture2D( RGB_SPECULAR_A_GLOSSNESS_TEXTURE , pvo.texcoord.xy ) );
		else 
			buf[2] = vec4( 0.0 );
		//////////	
		buf[0] = vec4( difo.xyz , ne.w );
	}else
	{
		buf[1] = vec4( pvo.normal , pvo.depth );
		buf[2] = vec4( 0.0 );
		buf[3] = vec4( vec3( 1.0 ) , 0.0 );
	}
}  
