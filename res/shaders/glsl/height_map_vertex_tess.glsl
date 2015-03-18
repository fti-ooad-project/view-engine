#version 430
$include uniforms.glsl
layout( location = 0 ) in vec3 invertex_position;
$include pervertex.glsl
out PerVertex pv;
vec2 grad( const vec2 tx , const float r , sampler2D Buffer_tex )
{
    float hx1 = texture2D( Buffer_tex , tx - vec2( r , 0.0 ) ).z;
    float hx2 = texture2D( Buffer_tex , tx + vec2( r , 0.0 ) ).z;
    float hy1 = texture2D( Buffer_tex , tx - vec2( 0.0 , r ) ).z;
    float hy2 = texture2D( Buffer_tex , tx + vec2( 0.0 , r ) ).z;
	return vec2( hx2 - hx1 , hy2 - hy1 );
}
const float DR = 1.0 / 1024.0;
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
	float h = texture2D( RGB_NORMAL_A_HEIGHT_TEXTURE , 0.5 + 0.5 * invertex_position.xy ).x;
	vec4 p = vec4( 500.0 * invertex_position.xy , ( h - 0.5 ) * 100.0 , 1.0 );
	pv.texcoord = 0.5 + 0.5 * invertex_position.xy;
	gl_Position = p;
	switch( PASSID )
	{
		case PASS_LIGHT:
		break;
		case PASS_WATER:
		break;
		case PASS_NORMAL:
			vec2 grad = grad( pv.texcoord , DR , RGB_NORMAL_A_HEIGHT_TEXTURE );
			float lg = length( grad );
			if( lg < 0.0001 )
			{
				pv.normal = vec3( 0.0 , 0.0 , 1.0 );
			}else
			{
				vec2 dir = normalize( grad );
				float h2 = texture2D( RGB_NORMAL_A_HEIGHT_TEXTURE , pv.texcoord + dir * DR ).x;
				vec3 pn = normalize( vec3( dir , ( h2 - h ) * 100 ) );
				pv.normal = - normalize( cross( cross( vec3( 0.0 , 0.0 , 1.0 ) , pn ) , pn ) );
			}
		break;
		default:
		break;
	}
	//pvo.normal = vec3( 0.0 , 0.0 , 1.0 );
	//vec4 pos = MAT4X4_VIEWPROJ[0] * vec4( p.xyz , 1.0 );
	//pvo.depth = pos.z;
} 
