#version 430
$include uniforms.glsl
$include pervertex.glsl
$include include.glsl
layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 3) out;
in PerVertex pvte[];
out PerVertex pvo;
/*vec2 grad( const vec2 tx , const float r , sampler2D Buffer_tex )
{
    float hx1 = texture2D( Buffer_tex , tx - vec2( r , 0.0 ) ).z;
    float hx2 = texture2D( Buffer_tex , tx + vec2( r , 0.0 ) ).z;
    float hy1 = texture2D( Buffer_tex , tx - vec2( 0.0 , r ) ).z;
    float hy2 = texture2D( Buffer_tex , tx + vec2( 0.0 , r ) ).z;
	return vec2( hx2 - hx1 , hy2 - hy1 );
}*/
const float DR = 1.0 / 1024.0;
void main()
{
    for( int i = 0; i < 3; ++i )
	{
		vec4 p;// = gl_in[i].gl_Position;
		float h = texture2D( RGB_NORMAL_A_HEIGHT_TEXTURE , pvte[i].texcoord ).x;
		float height = ( h - 0.5 ) * 100;
		p = MAT4X4_VIEWPROJ[0] * vec4( gl_in[i].gl_Position.xy , height , 1.0 );
		if( bool( LIGHTPASS ) )
		{
			gl_Position = scalel( p );
        }else
		{
			pvo.normal = pvte[i].normal;
			pvo.depth = p.z;
			pvo.texcoord = pvte[i].texcoord * 100.0;
			/*vec2 grad = grad( pvo.texcoord , DR , RGB_NORMAL_A_HEIGHT_TEXTURE );

			float lg = length( grad );
			if( lg < 0.0001 )
			{
				pvo.normal = vec3( 0.0 , 0.0 , 1.0 );
			}else
			{
				vec2 dir = normalize( grad );
				float h2 = texture2D( RGB_NORMAL_A_HEIGHT_TEXTURE , pvte[i].texcoord + dir * DR ).x;
				vec3 pn = normalize( vec3( dir , ( h2 - h ) * 100 ) );
				pvo.normal = - normalize( cross( cross( vec3( 0.0 , 0.0 , 1.0 ) , pn ) , pn ) );
			}*/
			gl_Position = p;
		}
		EmitVertex();
    }
	EndPrimitive();
}