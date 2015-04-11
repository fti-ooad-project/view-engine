#version 430

$include uniforms.glsl
$include include.glsl
$include pervertex.glsl
in PerVertex pvo;

out uvec4 buf;

void main()
{
	switch( PASSID )
	{
		case PASS_REFLECT:
			if( pvo.position.z < WATER_POS_SIZE.z ) discard;
		break;
		case PASS_WATER:
			if( pvo.position.z < WATER_POS_SIZE.z ) discard;
			buf = uvec4( 1 );
		break;
		case PASS_LIGHT:
			buf = uvec4( 1 );
		break;
		case PASS_NORMAL:
			vec3 diff = vec3( 0.5 );
			vec4 norm = vec4( pvo.normal , pvo.depth );
			vec4 spec = vec4( 0.0 );
			vec3 grass = texture( TEXTURE_ARRAY , vec3( pvo.texcoord , 1.0 ) ).xyz;
			vec3 snow = texture( TEXTURE_ARRAY , vec3( pvo.texcoord , 2.0 ) ).xyz;
			vec3 dirt = texture( TEXTURE_ARRAY , vec3( pvo.texcoord , 3.0 ) ).xyz;
			const float lvl = 5.0;
			if( pvo.position.z < lvl )
			{
				diff = mix( grass , dirt , min( 1.0 , abs( lvl - pvo.position.z ) / 5.0 ) );
			}else
			{
				diff = mix( grass , snow , min( 1.0 , abs( pvo.position.z - lvl ) / 5.0 ) );
			}
			
			/*vec4 ne = texture( TEXTURE_ARRAY , vec3( pvo.texcoord , 0.0 ) );
			vec4 bump = -1.0 + ne * 2.0;
			vec3 newnormal = normalize( pvo.normal * bump.z-pvo.binormal * bump.y + pvo.tangentnormal * bump.x );
			norm = vec4( newnormal.xyz , pvo.depth);*/
			
			buf.x = uint( pvo.depth * 100.0 );
			buf.y = pack4i( vec4( diff , pvo.select_id ) );
			float sg = norm.z >= 0.0 ? 1.0 : -1.0;
			buf.z = pack4i( vec4( 0.5 + 0.5 * norm.xy , 0.5 + 0.5 * sg , 0.0 ) );
			buf.w = pack4i( spec );
		break;
		default:
		break;
	}
}  