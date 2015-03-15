#version 430
layout( location = 0 ) uniform sampler2D CUR_BUF;
layout( location = 1 ) uniform sampler2D LAST_BUF;
layout( location = 2 ) uniform int PASS;
layout( location = 3 ) uniform sampler2D Buffer_tex;
layout( location = 4 ) uniform vec2 WindowRatio;
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
void main()
{
	if( PASS == 0 )
	{
		float dr = 1.0 * WindowRatio.x;
		vec4 cur = texture2D( CUR_BUF , frag_pos );
		vec4 last = texture2D( LAST_BUF , frag_pos );
		vec4 buf = texture2D( Buffer_tex , frag_pos );
		buf += vec4( vec2( 0.0 ) , abs( cur.x - last.x ) , 0.0 );
		vec2 pg = grad( frag_pos , dr );
		float pdiv = div( frag_pos , dr );
		buf += vec4( 0.2 * ( -pg ) , ( -buf.z * 0.9 + pdiv ) * 0.3 , 0.0 );
		//vec4 nbuf = texture2D( Buffer_tex , frag_pos - buf.xy * 0.01 );
		out_data = vec4( buf.xyz , 1.0 );
		return;
	}
	vec4 buf = texture2D( Buffer_tex , frag_pos );
	vec3 v = normalize( vec3( -buf.xy , 0.6 ) );
	out_data = vec4( 0.5 + 0.5 * v , 1.0 );
}