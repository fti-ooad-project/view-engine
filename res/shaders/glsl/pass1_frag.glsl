#version 430
$include include.glsl
$include light_func.glsl
layout( location = 0 ) uniform sampler2D BUFFER0;//pos cull
layout( location = 1 ) uniform sampler2D BUFFER1;//normal depth
layout( location = 2 ) uniform sampler2D BUFFER2;//spec gloss
layout( location = 3 ) uniform sampler2D BUFFER3;//diff emiss
layout( location = 4 ) uniform Camera CAM;
layout( location = 9 ) uniform int OLIGHT_COUNT;
layout( location = 10 ) uniform int DLIGHT_COUNT;
layout( location = 20 ) uniform OmniLight OLIGHT[MAX_LIGHTS];
layout( location = 60 ) uniform DirectedLight DLIGHT[MAX_LIGHTS];

layout( location = 11 ) uniform sampler2D ANIM_TEX;

in vec2 frag_pos;
out vec4 out_data;
vec3 light( vec3 p , vec3 n , vec3 v )
{
	vec3 lightness = vec3( 0.0 );
	vec3 refl = reflect( normalize( p - v ) , n );
	
	/*for( int i = 0; i < 1; i++ )//COUNT!!!
	{
		vec3 to_light = p - OLIGHT[i].pos;
		float k;
		if( dot( n , n ) == 0.0 )
			k = 1.0;
		else
			k = pow( max( 0.0 , dot( refl , -to_light ) ) , 1.0 );
		float r2 = 1.0 / dot( to_light , to_light );
		lightness += OLIGHT[i].colori.xyz * OLIGHT[i].colori.w *
		smoothLightCube( p , n , 1.0 , OLIGHT[i].pos , OLIGHT[i].DepthCubeMap_Buffer ) * k * r2 * 5.0;
	}*/
	for( int i = 0; i < 1; i++ )//COUNT!!!
	{
		float k;
		if( dot( n , n ) == 0.0 )
			k = 1.0;
		else
			k = max( 0.0 , dot( refl , -DLIGHT[i].dir ) );
		lightness += DLIGHT[i].colori.xyz * DLIGHT[i].colori.w *
		smoothLightDir( p , n , 0.1 , DLIGHT[i].toLightViewProj , DLIGHT[i].DepthMap_Buffer ) * k;
	}
	return lightness.xyz;
}
/*vec3 fog( vec3 p , vec3 v )
{
	const int ITER = 20;
	const float D = 40.0;
	const float DR = D / ITER;
	vec3 out_fog = vec3( 0.0 );
	for( float r = DR; r < D; r += DR )
	{
		vec3 np = p + v * r;
		out_fog += light( np , vec3( 0.0 ) ) * exp( -r );
	}
	return out_fog;
}*/
void main()
{
	//out_data = vec4( pow( texture2D( DLIGHT[0].DepthMap_Buffer , frag_pos ).x , 1.0 ) ); return;
	definePoisson();
	const vec4 buf0 = texture2D( BUFFER0 , frag_pos );
	if( buf0.w < 0.5 )
	{
		out_data = vec4( 0.0 );
		//return;
		discard;
	}
	//out_data = vec4(1.0  / dfdr( frag_pos , 0.004 , BUFFER1 ).w);return;
	const vec3 pos = buf0.xyz;
	vec4 buf1 = texture2D( BUFFER1 , frag_pos );
	//vec4 buf2 = texture2D( BUFFER2 , frag_pos );
	vec4 buf3 = texture2D( BUFFER3 , frag_pos );
	/*if( 1.0 / dfdr( frag_pos , 0.001 , BUFFER1 ).w < 0.8 )
	{
		out_data = vec4( 0.0 );
		return;
	}*/
	//vec3 ray = camRay( CAM , frag_pos );
	//vec3 fg = fog( CAM.pos , ray );
	vec3 norm = buf1.xyz;
	//vec3 l = light( pos , norm , CAM.pos );//* vec3( ssao( BUFFER1 , buf1 , frag_pos , 0.1 ) ) 
	out_data = //abs( texture2D( ANIM_TEX , frag_pos ) );//
	vec4(  buf3.xyz * max( 0.0 , dot( norm , vec3( 0.0 , 0.0 , 1.0 ) ) ) , 1.0 );//vec4( 1.0 );//vec4( pow( texture2D( DLIGHT[0].DepthMap_Buffer , frag_pos ).x  , 4.0 ) );//
	
}//abs( getMatFromTex( frag_pos.y , frag_pos.x * 21.0 - fract( frag_pos.x * 21.0 ) , 21 , ANIM_TEX )[0] );
//abs( texture2D( ANIM_TEX , frag_pos ) );abs( texture2D( ANIM_TEX , frag_pos ) );
//vec4( pow( texture2D( DLIGHT[0].DepthMap_Buffer , frag_pos ).x , 1.0 ) );
//vec4( lightedDir( DLIGHT[0].toLightViewProj * buf0 , DLIGHT[0].DepthMap_Buffer ) );