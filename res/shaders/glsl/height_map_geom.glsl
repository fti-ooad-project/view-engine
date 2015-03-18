#version 430
$include uniforms.glsl
$include pervertex.glsl
$include include.glsl
layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 4) out;
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
vec3 getColwithWater( vec3 p0 , vec3 p1 , float waterlevel )
{
	vec3 v = p1 - p0;
	float ds = waterlevel - p0.z;
	float dz = p1.z - p0.z;
	return p0 + v * ds / dz;
}
const float DR = 1.0 / 1024.0;
void main()
{
	int p = 0 , n = 0;
	float height[3];
	vec4 pos[3];
	for( int i = 0; i < 3; ++i )
	{
		float h = texture2D( RGB_NORMAL_A_HEIGHT_TEXTURE , pvte[i].texcoord ).x;
		height[i] = ( h - 0.5 ) * 100;
		pos[i] = vec4( gl_in[i].gl_Position.xy , height[i] , 1.0 );
		if( height[i] > WATERZ )
			p++;
		else
			n++;
	}
	bool temp = false;
    for( int i = 0; i < 3; ++i )
	{
		switch( PASSID )
		{
			case PASS_LIGHT:
				gl_Position = scalel( MAT4X4_VIEWPROJ[0] * pos[i] );
				EmitVertex();
			break;
			case PASS_WATER:
				if( n != 3 )
				{
					if( p == 1 )
					{
						if( height[i] > WATERZ )
						{
							gl_Position = MAT4X4_VIEWPROJ[0] * pos[i];
							EmitVertex();
						}else
						{
							gl_Position = MAT4X4_VIEWPROJ[0] * vec4( getColwithWater( pos[i].xyz , pos[(i + 1)%3].z > WATERZ ? pos[(i + 1)%3].xyz : pos[(i + 2)%3].xyz , WATERZ ) , 1.0 );
							EmitVertex();
						}
					}else
					if( p == 2 )
					{
						if( height[i] > WATERZ )
						{
							gl_Position = MAT4X4_VIEWPROJ[0] * pos[i];
							EmitVertex();
						}else
						{
							gl_Position = MAT4X4_VIEWPROJ[0] * vec4( getColwithWater( pos[i].xyz , pos[(i + 1)%3].xyz , WATERZ ) , 1.0 );
							EmitVertex();
							gl_Position = MAT4X4_VIEWPROJ[0] * vec4( getColwithWater( pos[i].xyz , pos[(i + 2)%3].xyz , WATERZ ) , 1.0 );
							EmitVertex();
						}
					}else
					{
						gl_Position = MAT4X4_VIEWPROJ[0] * pos[i];
						EmitVertex();
					}
				}
			break;
			case PASS_NORMAL:
				gl_Position = MAT4X4_VIEWPROJ[0] * pos[i];
				pvo.normal = pvte[i].normal;
				pvo.depth = gl_Position.z;
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
				
				EmitVertex();
			break;
			default:
			break;
		}
    }
	EndPrimitive();
}