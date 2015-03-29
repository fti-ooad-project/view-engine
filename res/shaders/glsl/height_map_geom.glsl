#version 430
$include uniforms.glsl
$include pervertex.glsl
$include include.glsl
layout(triangles, invocations = 1) in;
layout(triangle_strip, max_vertices = 4) out;
in PerVertex pvte[];
out PerVertex pvo;
vec3 norm( const vec2 tx , const float r , sampler2D Buffer_tex )
{
    vec3 o = vec3( 0.0 );
    float k = 0.125;
    float hx1 = texture2D( Buffer_tex , tx - vec2( r * 0.5 , 0.0 ) ).x;
    float hx2 = texture2D( Buffer_tex , tx + vec2( r * 0.5 , 0.0 ) ).x;
    float hy1 = texture2D( Buffer_tex , tx - vec2( 0.0 , r * 0.5 ) ).x;
    float hy2 = texture2D( Buffer_tex , tx + vec2( 0.0 , r * 0.5 ) ).x;

    return normalize( cross( vec3( vec2( r , 0.0 ) , k * ( hx2 - hx1 ) )  , vec3( vec2( 0.0 , r ) , k * ( hy2 - hy1 ) ) ) );
}
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
	//int p = 0 , n = 0;
	float height[3];
	vec4 pos[3];
	for( int i = 0; i < 3; ++i )
	{
		float h = texture( BUFFER1 , pvte[i].texcoord , 0.0 ).x;
		height[i] = ( h - 0.5 ) * 50;
		pos[i] = vec4( gl_in[i].gl_Position.xy , height[i] , 1.0 );
		/*if( height[i] > WATERZ )
			p++;
		else
			n++;*/
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
			case PASS_REFLECT:
			case PASS_WATER:
			{
				gl_Position = MAT4X4_VIEWPROJ[0] * pos[i];
				pvo.position = pos[i].xyz;
				EmitVertex();
				/*if( n != 3 )
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
				}*/
			}
			break;
			case PASS_NORMAL:
				gl_Position = MAT4X4_VIEWPROJ[0] * pos[i];
				float camdist = distance( pos[i].xyz , CAM_POS );//max( 0.0 , min( 1.0 , camdist / 50.0 - 2.0 ) )
				pvo.normal = norm( pvte[i].texcoord , mix( DR , 10.0 * DR , max( DR , 0.01 * exp( camdist * 0.03 ) ) ) , BUFFER1 );
				pvo.depth = gl_Position.z;
				pvo.texcoord = pvte[i].texcoord * 100.0;
				pvo.position = pos[i].xyz;
				EmitVertex();
			break;
			default:
			break;
		}
    }
	EndPrimitive();
}