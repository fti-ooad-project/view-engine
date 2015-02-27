const int MASK_TEXTURED			= 1 << 0;
const int MASK_TEXTURED_DIF			= 1 << 1;
const int MASK_TEXTURED_NOR 	= 1 << 2;
const int MASK_TEXTURED_SPE		= 1 << 3;
const int MASK_ANIMATED			= 1 << 4;
const int MAX_LIGHTS 			= 10;
#define pi 3.1415
const int NUM_TAPS = 12;
vec2 poisson[NUM_TAPS];
struct Camera
{
	vec3 pos;
	vec3 look;
	vec3 left;
	vec3 up;
};
vec3 camRay( struct Camera cam , vec2 tx )
{
	vec2 p2 = vec2( 2.0 , -2.0 ) * tx - 1.0;
	return normalize( cam.look + cam.left * p2.x + cam.up * p2.y );
}
float wrap( float d )
{
	return max( 0.0 , min( 1.0 , d ) );
}
void definePoisson()
{
	poisson[0]  = vec2( -0.326 , -0.406 );
	poisson[1]  = vec2( -0.84 , -0.074 );
	poisson[2]  = vec2( -0.696 , 0.457 );
	poisson[3]  = vec2( -0.203 , 0.621 );
	poisson[4]  = vec2( 0.962 , -0.195 );
	poisson[5]  = vec2( 0.473 , -0.480 );
	poisson[6]  = vec2( 0.519 , 0.767 );
	poisson[7]  = vec2( 0.185 , -0.893 );
	poisson[8]  = vec2( 0.507 , 0.064 );
	poisson[9]  = vec2( 0.896 , 0.412 );
	poisson[10] = vec2( -0.322 , -0.933 );
	poisson[11] = vec2( -0.792 , -0.598 );
}
mat4 getMatFromTex( float time , float bone , float bone_count , sampler2D anim_buffer )
{
	vec2 step = vec2( 1.0 / ( 4.0 * bone_count ) , 0.0 );
	vec2 tx = vec2( ( bone ) * step.x * 4.0 + step.x * 0.5 , time );
	return mat4(
		texture2D( anim_buffer , tx ) ,
		texture2D( anim_buffer , tx + step ) ,
		texture2D( anim_buffer , tx + 2.0 * step ) ,
		texture2D( anim_buffer , tx + 3.0 * step ) );
}
vec4 dfdr( vec2 tx , float dr , sampler2D tex )
{
	vec4 dfdx = abs( texture2D( tex , tx + vec2( dr , 0.0 ) ) - texture2D( tex , tx + vec2( -dr , 0.0 ) ) );
	vec4 dfdy = abs( texture2D( tex , tx + vec2( 0.0 , dr ) ) - texture2D( tex , tx + vec2( 0.0 , -dr ) ) );
	return dfdx + dfdy;
}
float nrand( vec2 n )
{
	return fract( sin( dot( n.xy , vec2( 12.9898 , 78.233 ) ) ) * 43758.5453 );
}
vec2 rot2d( vec2 p , float a )
{
	vec2 sc = vec2( sin( a ) , cos( a ) );
	return vec2( dot( p , vec2( sc.y , -sc.x ) ) , dot( p , sc.xy ) );
}
vec3 nfrom2d( vec2 v )
{
	float ca = cos( 2.0 * pi * v.x );
	float sa = sin( 2.0 * pi * v.x );
	float ct = v.x;
	float st = sqrt( 1 - ct * ct );
	return vec3( st * ca , st * sa , ct ); 
}
float pack4f( vec4 data )
{
	return dot( data , vec4( 1.0 , 1 / 255.0 , 1 / 65025.0 , 1 / 160581375.0 ) );
}
vec4 unpack4f( float data )
{
	vec4 outv = vec4( 1.0 , 255.0 , 65025.0 , 160581375.0 ) * data;
	outv = fract( outv );
	return outv - outv.yzww * vec4( 1.0 / 255.0 , 1.0 / 255.0 , 1.0 / 255.0 , 0.0 );
}
float ssao( sampler2D norm_depth_buf , vec4 norm_depth , vec2 tx , float radius )
{
	float o = 0.0;
	for( int i = 0; i < NUM_TAPS; ++i )
	{
		vec4 temp = texture2D( norm_depth_buf , tx + rot2d( poisson[i] , nrand( tx * 123.0 ) ) * radius / norm_depth.w );// 
		if( abs( temp.w - norm_depth.w ) > 1.0 || temp.w > norm_depth.w - 0.1 )//|| dot( temp.xyz , norm_depth.xyz ) > 0.2 )
			o += 1.0;
	}
	return o / NUM_TAPS;
}
float lfunc( float p )
{
	return sin( p * pi * 0.5 );
}
vec4 scalel( vec4 p )
{
	p /= p.w;
	return p;//vec4( vec2( lfunc( p.x ) , lfunc( p.y ) ) , p.zw );
}