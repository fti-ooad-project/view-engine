struct OmniLight
{
    vec3 pos;
    vec4 colori;
	bool shadow;
    samplerCube DepthCubeMap_Buffer;
};
struct DirectedLight
{
    vec3 dir;
    vec4 colori;
	mat4 toLightViewProj;
    sampler2D DepthMap_Buffer;
};
float VectorToDepthValue( vec3 v )
{
    vec3 av = abs( v );
    float z = max( av.x , max( av.y , av.z ) );
    float f = 1000.0;
    float n = 1.0;
    float nz = f / ( f - n ) * ( 1.0 - n / z );
    return ( 1.0 + nz ) * 0.5;
}
float lightedDir( vec4 loc , sampler2D tex )
{
    //loc = loc / loc.w;
	if( abs( loc.x ) > 1.0 || abs( loc.y ) > 1.0 ) return 1.0;
	loc = ( 1.0 + loc ) * 0.5;
	//return abs( loc.z - texture2D( tex , loc.xy ).x );
	if( loc.z - 0.0001 < texture2D( tex , loc.xy ).x ) return 1.0;
    else return 0.0;
}
float lightedCube( vec3 ftol , samplerCube cubemap )
{
    vec4 fw = texture( cubemap , normalize( ftol ) );
	return ( fw.x - VectorToDepthValue( ftol ) );
    //if( fw.x + 0.00005 > VectorToDepthValue( ftol ) ) return 1.0;
    //else return 0.0;
}
float smoothLightDir( vec3 p , vec3 n , float samp_r , mat4 vp , sampler2D tex )
{
	float o = 0.0;
	for( int i = 0; i < NUM_TAPS; ++i )
	{
		vec3 rand = nfrom2d( rot2d( poisson[i] , nrand( p.xy * 123.0 ) ) );
		if( dot( rand , n ) < 0.0 ) rand *= -1.0;
		rand = rand + n * 0.7;
		vec3 np = p + rand * samp_r;
		o += lightedDir( scalel( vp * vec4( np , 1.0 ) ) , tex );
		//o += lightedDir( scalel( vp * vec4( rand , 1.0 ) ) , tex ) > -0.001? 1.0 : 0.0;
	}
	return o / NUM_TAPS;
}
float smoothLightCube( vec3 p , vec3 n , float samp_r , vec3 lp , samplerCube tex )
{
	float o = 0.0;
	float rad = clamp( -20.0 * ( lightedCube( p - lp , tex ) ) , 0.0 , samp_r );
	for( int i = 0; i < NUM_TAPS; ++i )
	{//p + normalize( n + 0.5 * 
		vec3 rand = nfrom2d( rot2d( poisson[i] , nrand( p.xy * 123.0 ) ) );
		if( dot( rand , n ) < 0.0 ) rand *= -1.0;
		rand = rand + n * 0.7;
		vec3 np = p + rand * rad;
		o += lightedCube( np - lp , tex ) > -0.001? 1.0 : 0.0;
	}
	return o / NUM_TAPS;
}