const int MASK_TEXTURED			= 1 << 0;
const int MASK_TEXTURED_DIF			= 1 << 1;
const int MASK_TEXTURED_NOR 	= 1 << 2;
const int MASK_TEXTURED_SPE		= 1 << 3;
const int MASK_ANIMATED			= 1 << 4;
#define MAX_LIGHTS_CASTER 3
#define MAX_LIGHTS 10
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
vec3 posFromZ( vec2 tx , float z , struct Camera cam )
{
	return ( cam.look + cam.left * tx.x + cam.up * tx.y ) * z + cam.pos;
}
vec3 camRay( struct Camera cam , vec2 tx )
{
	vec2 p2 = vec2( 2.0 , 2.0 ) * tx - 1.0;
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
const float _x = 1.0;
const float _y = 256.0;//255.0;
const float _z =  _y * _y;
const float _w = _y * _y * _y;
vec4 roundf4( vec4 a )
{
	a = floor( a * 255.0 ) / 256.0;
	return a;
}
float pack4f( vec4 data )
{
	return dot( roundf4( data ) , vec4( 1.0 , 1.0 / _y , 1.0 / _z , 1.0 / _w ) );
}
vec4 unpack4f( float data )
{
	vec4 outv = vec4( _x , _y , _z , _w ) * data;
	outv = fract( outv );
	outv -= outv.yzww * vec4( 1.0 / _y , 1.0 / _y , 1.0 / _y , 0.0 );
	return outv;
}
uint pack4i( vec4 data )
{
	uint outv = 0;
	for( int i = 0; i < 4; i++ )
		outv |= int( data[i] * 255.0 ) << ( i * 8 );
	return outv;
}
vec4 unpack4i( uint data )
{
	vec4 outv;
	for( int i = 0; i < 4; i++ )
		outv[i] = float( ( data & ( 255 <<  ( i * 8 ) ) ) >> ( i * 8 ) ) / 255.0;
	return outv;
}
float depthFromi( uvec4 r )
{
	return float( r.x ) / 100.0;
}
vec3 normFromi( uvec4 r )
{
	vec4 unpacked_n = -1.0 + 2.0 * unpack4i( r.z );
	return vec3( unpacked_n.xy , sign( unpacked_n.z ) * sqrt( max( 0.0 , 1.0 - pow( abs( unpacked_n.x ) , 2.0 ) - pow( abs( unpacked_n.y ) , 2.0 ) ) ) );
}
float ssao( usampler2D norm_depth_buf , vec4 norm_depth , vec2 tx , float radius )
{
	float o = NUM_TAPS;
	for( int i = 0; i < NUM_TAPS; ++i )
	{
		vec2 ntx = tx + rot2d( poisson[i] , nrand( tx ) * 2.0 * pi ) * radius / norm_depth.w;
		if( ntx.x > 1.0 || ntx.x < 0.0 || ntx.y > 1.0 || ntx.y < 0.0 )
		{
			continue;
		}else
		{
			uvec4 temp = texture( norm_depth_buf , ntx );
			float depth = depthFromi( temp );
			vec3 norm = normFromi( temp );
			if(
			norm_depth.w - depth > 0.0
			&& dot( norm , norm_depth.xyz ) < 0.7
			&&  norm_depth.w - depth < 1.0
			)
				o -= 1.0;
		}
	}
	return o / NUM_TAPS;
}
float lfunc( float p )
{
	return sign( p ) * ( 1.0 - pow( 1.0 - abs( p ) , 10.0 ) );
}
vec4 scalel( vec4 p )
{
	p /= p.w;
	if( abs( p.x ) > 1.0 || abs( p.y ) > 1.0 ) return p;
	return vec4( vec2( lfunc( p.x ) , lfunc( p.y ) ) , p.zw );
}