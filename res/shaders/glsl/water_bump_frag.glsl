#version 430
layout( location = 0 ) uniform usampler2D CUR_BUF;
layout( location = 1 ) uniform usampler2D LAST_BUF;
layout( location = 2 ) uniform int PASS;
layout( location = 3 ) uniform sampler2D Buffer_tex;
layout( location = 5 ) uniform sampler2D Wave_tex;
layout( location = 4 ) uniform vec2 WindowRatio;
layout( location = 6 ) uniform float TIME;
layout( location = 7 ) uniform vec2 CAM_DR;
layout( location = 8 ) uniform float DT;
in vec2 frag_pos;
out vec4 out_data;
vec2 grad( const vec2 tx , const float r )
{
    float hx1 = texture2D( Buffer_tex , tx - vec2( r , 0.0 ) ).z;
    float hx2 = texture2D( Buffer_tex , tx + vec2( r , 0.0 ) ).z;
    float hy1 = texture2D( Buffer_tex , tx - vec2( 0.0 , r ) ).z;
    float hy2 = texture2D( Buffer_tex , tx + vec2( 0.0 , r ) ).z;
	return vec2( hx2 - hx1 , hy2 - hy1 );
}
float div( const vec2 tx , const float r )
{
    vec2 hx1 = texture2D( Buffer_tex , tx - vec2( r , 0.0 ) ).xy;
    vec2 hx2 = texture2D( Buffer_tex , tx + vec2( r , 0.0 ) ).xy;
    vec2 hy1 = texture2D( Buffer_tex , tx - vec2( 0.0 , r ) ).xy;
    vec2 hy2 = texture2D( Buffer_tex , tx + vec2( 0.0 , r ) ).xy;
	return hx1.x - hx2.x + hy1.y - hy2.y;
}
vec3 norm( const vec2 tx , const float r )
{
    vec3 o = vec3( 0.0 );
    float k = 0.001;
    float hx1 = texture2D( Buffer_tex , tx - vec2( r * 0.5 , 0.0 ) ).z;
    float hx2 = texture2D( Buffer_tex , tx + vec2( r * 0.5 , 0.0 ) ).z;
    float hy1 = texture2D( Buffer_tex , tx - vec2( 0.0 , r * 0.5 ) ).z;
    float hy2 = texture2D( Buffer_tex , tx + vec2( 0.0 , r * 0.5 ) ).z;

    return normalize( cross( vec3( vec2( r , 0.0 ) , k * ( hx2 - hx1 ) )  , vec3( vec2( 0.0 , r ) , k * ( hy2 - hy1 ) ) ) );
}
vec2 laplas( const vec2 tx , const float r ){
	vec2 v = texture2D( Buffer_tex , tx ).xy;
	vec2 dvdx = texture2D( Buffer_tex , tx + vec2( r , 0.0 ) ).xy + texture2D( Buffer_tex , tx - vec2( r , 0.0 ) ).xy;
    vec2 dvdy = texture2D( Buffer_tex , tx + vec2( 0.0 , r ) ).xy + texture2D( Buffer_tex , tx - vec2( 0.0 , r ) ).xy;
	return vec2( dvdx.x + dvdy.x - 4.0 * v.x , dvdx.y + dvdy.y - 4.0 * v.y );
}
float PPoison( const vec2 tx , const float r ){
	float dpdx = texture2D( Buffer_tex , tx + vec2( r , 0.0 ) ).z + texture2D( Buffer_tex , tx - vec2( r , 0.0 ) ).z;
    float dpdy = texture2D( Buffer_tex , tx + vec2( 0.0 , r ) ).z + texture2D( Buffer_tex , tx - vec2( 0.0 , r ) ).z;
	return ( dpdx + dpdy + div( tx , r ) ) * 0.25;
}
vec2 VPoison( const vec2 tx , const float r ){
	vec2 v = texture2D( Buffer_tex , tx ).xy;
	vec2 dx = texture2D( Buffer_tex , tx - vec2( r , 0.0 ) ).xy + texture2D( Buffer_tex , tx + vec2( r , 0.0 ) ).xy;
    vec2 dy = texture2D( Buffer_tex , tx - vec2( 0.0 , r ) ).xy + texture2D( Buffer_tex , tx + vec2( 0.0 , r ) ).xy;
	float a = 0.01;
	float b = 4.0 + a;
	return ( dx + dy + v * a ) / b;
}
vec3 normfrp( const vec2 tx , const float r )
{
	return normalize( -cross(
	vec3( 0.0 , 2.0 * r , -0.01 * ( texture2D( Buffer_tex , tx + vec2( r , 0.0 ) ).z - texture2D( Buffer_tex , tx - vec2( r , 0.0 ) ).z ) )
	, vec3( 2.0 * r , 0.0 , -0.01 * ( texture2D( Buffer_tex , tx + vec2( 0.0 , r ) ).z - texture2D( Buffer_tex , tx - vec2( 0.0 , r ) ).z ) )
	) );
}
void main()
{
	float dr = 1.0 * WindowRatio.x;
	if( PASS == 0 )
	{
				/*if( frag_pos.y > 0.9 )
		{
			out_data = vec4( vec2( 0.0 , -0.1 ) , 0.1 , 0.0 );
			return;
		}*/
		vec2 newfragpos = frag_pos + CAM_DR.xy;
		float cur = float( texture( CUR_BUF , frag_pos ).x );
		float last = float( texture( LAST_BUF , newfragpos ).x );
		//bool still = length( CAM_DR ) < 0.001; 
		
		if( cur > 0.8 )
		{
			out_data = vec4( 0.0 , 0.0 , ( 0.8 + 0.2 * sin( TIME * 10.0 ) ) * 0.2 + abs( cur - last ) * 0.2 , 0.0 );
			return;
		}
		vec4 buf = texture2D( Buffer_tex , newfragpos );
		buf = texture2D( Buffer_tex , newfragpos - buf.xy * DT );
		buf.z += 0.2 * abs( cur - last );
		vec2 pg = grad( newfragpos , dr );
		float udiv = div( newfragpos , dr );// - buf.xy * 0.2 udiv * 0.2 
		buf += 40.0 * vec4( vec2( 0.0 , 0.001 ) -0.2 * pg - buf.xy * 0.02 , - buf.z * 0.1 + udiv * 0.1 , 0.0 ) * DT;
		out_data = buf;
		return;
	}
	/*vec4 buf = texture2D( Buffer_tex , frag_pos );
	float time = TIME * 0.01 + frag_pos.x + frag_pos.y;
	vec4 wnorm = texture2D( Wave_tex , frag_pos * 70.0 + vec2( sin( time ) , cos( time ) ) * 5.0  );
	vec4 wnorm1 = texture2D( Wave_tex , frag_pos * 70.0 - vec2( cos( time ) , sin( time ) ) * 5.0  );
	vec3 wavenorm = wnorm.xyz;
	vec3 wavenorm1 = wnorm1.xyz;*/
	vec3 v = /*wavenorm + wavenorm1 + */normfrp( frag_pos , dr );
	//v.z = abs( v.z );
	out_data = vec4( 0.5 + 0.5 * normalize( v ) , 1.0 );
}