#version 430
$include include.glsl
$include light_func.glsl
layout( location = 0 ) uniform usampler2D BUFFER0;//pos cull
//layout( location = 1 ) uniform sampler2D BUFFER1;//normal depth
//layout( location = 2 ) uniform sampler2D BUFFER2;//spec gloss
//layout( location = 3 ) uniform sampler2D BUFFER3;//diff emiss
//layout( location = 11 ) uniform sampler2D WATER_BUFFER3;//diff emiss
layout( location = 4 ) uniform Camera CAM;
layout( location = 9 ) uniform int OLIGHT_COUNT;
layout( location = 10 ) uniform int DLIGHT_COUNT;
layout( location = 20 ) uniform OmniLight OLIGHT[MAX_LIGHTS_CASTER];
layout( location = 32 ) uniform DirectedLight DLIGHT[MAX_LIGHTS_CASTER];

//layout( location = 11 ) uniform sampler2D ANIM_TEX;

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
		else//+ max( 0.0 , dot( refl , -DLIGHT[i].dir ) )
			k = pow( max( 0.0 , dot( n , -DLIGHT[i].dir )  ) , 0.8 );
		lightness += DLIGHT[i].colori.xyz * DLIGHT[i].colori.w *
		smoothLightDir( p , n , 0.1 , DLIGHT[i].toLightViewProj , DLIGHT[i].DepthMap_Buffer ) * 
		k;
	}
	return lightness.xyz;
}
void main()
{
	definePoisson();
	uvec4 buf0 = texture( BUFFER0 , frag_pos );
	float depth = float( buf0.x ) / 100.0; 
	//return;
	vec3 unpacked_n = -1.0 + 2.0 * unpack4i( buf0.z ).xyz;
	/*if( buf0.x > 10000 )
	{
		out_data = vec4( 0.0 );
		//return;
		discard;
	}*/
	vec3 l;
	vec3 pos = posFromZ( vec2( -1.0 + 2.0 * frag_pos.x , -1.0 + 2.0 * frag_pos.y ) , depth , CAM );
	vec3 norm = vec3( unpacked_n.xy , sign( unpacked_n.z ) * sqrt( max( 0.0 , 1.0 - pow( abs( unpacked_n.x ) , 2.0 ) - pow( abs( unpacked_n.y ) , 2.0 ) ) ) );
	vec3 diff = unpack4i( buf0.y ).xyz;
	l = vec3( 0.2 ) + light( pos , norm , CAM.pos );
	out_data = vec4( l * diff , 1.0 );
	//vec4( pow( texture2D( DLIGHT[0].DepthMap_Buffer , frag_pos ).x , 1.0 ) );
}