#version 430
$include include.glsl
$include light_func.glsl
layout( location = 0 ) uniform usampler2D BUFFER0;//pos cull
layout( location = 1 ) uniform sampler2D ENV;//normal depth
//layout( location = 2 ) uniform sampler2D BUFFER2;//spec gloss
//layout( location = 3 ) uniform sampler2D BUFFER3;//diff emiss
layout( location = 11 ) uniform usampler2D WATER_BUFFER3;//diff emiss
layout( location = 4 ) uniform Camera CAM;
layout( location = 9 ) uniform int OLIGHT_COUNT;
layout( location = 10 ) uniform int DLIGHT_COUNT;
layout( location = 20 ) uniform OmniLight OLIGHT[MAX_LIGHTS_CASTER];
layout( location = 32 ) uniform DirectedLight DLIGHT[MAX_LIGHTS_CASTER];

//layout( location = 11 ) uniform sampler2D ANIM_TEX;
//specw.r - metallness
//specw.a - exponent
in vec2 frag_pos;
out vec4 out_data;
vec3 env( vec3 n , float s )
{
	float alpha = atan( n.y , n.x );
	float theta = atan( n.z , length( n.xy ) );
	//return texture( ENV , vec2( 0.5 + alpha / pi * 0.5 , 0.5 - theta / pi ) ).xyz;
	//float mipmapLevel = textureQueryLod( ENV , textureCoord ).x;
	return textureLod( ENV , vec2( 0.5 + alpha / pi * 0.5 , 0.5 - theta / pi ) , s * 10.0 ).xyz;
}
vec3 light( vec3 p , vec3 n , vec3 v , vec4 specw )
{
	vec3 lightness = vec3( 0.0 );
	vec3 refl = reflect( normalize( p - v ) , n );
	float freshnel = mix( 1.0 , max( 0.0 , 1.0 - dot( -normalize( p - v ) , n ) ) , 1.0 - specw.x );
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
		else//+ 
			k = mix( freshnel * pow( max( 0.0 , dot( refl , -DLIGHT[i].dir ) ) , specw.w * 10.0 ) , max( 0.0 , dot( n , -DLIGHT[i].dir ) ) , 1.0 - specw.w );
		
		lightness += DLIGHT[i].colori.xyz * DLIGHT[i].colori.w *
		smoothLightDir( p , n , 0.1 , DLIGHT[i].toLightViewProj , DLIGHT[i].DepthMap_Buffer ) * 
		k;
	}
	lightness += freshnel * env( refl , 1.0 - specw.w );
	return lightness.xyz;
	//return vec3( smoothLightDir( p , n , 0.1 , DLIGHT[0].toLightViewProj , DLIGHT[0].DepthMap_Buffer ) );
}
void main()
{
	definePoisson();
	uvec4 buf0 = texture( BUFFER0 , frag_pos );
	float depth = depthFromi( buf0 );
	vec2 distfragpos = frag_pos;
	///
	uvec4 wbuf = texture( WATER_BUFFER3 , frag_pos );
	vec4 wY = unpack4i( wbuf.y );
	vec3 WATERK = vec3( 1.0 );
	if( wY.w > 0.0 )
	{
		
		float wdepth = depthFromi( wbuf );
		vec2 screenspacewaternorm = 2.0 * wY.xy - 1.0;
		distfragpos += screenspacewaternorm * 0.01;
		vec3 wpos = posFromZ( vec2( -1.0 + 2.0 * frag_pos.x , -1.0 + 2.0 * frag_pos.y ) , wdepth , CAM );
		vec4 wspecw = unpack4i( wbuf.w );
		vec3 wnorm = normFromi( wbuf );
		WATERK = vec3( 0.5 ) * dot( normalize( CAM.pos - wpos ) , wnorm ) + vec3( 0.1 , 0.1 , 0.1 );
		vec3 wl = light( wpos , wnorm , CAM.pos , wspecw );
		out_data += vec4( vec3( 0.5 , 0.7 , 0.9 ) * wl , 1.0 );//vec4( dot( wnorm , vec3( 0.0 , 0.0 , 1.0 ) ) );
	}
	///
	if( buf0.x == 0 )
	{
		vec3 v = camRay( CAM , frag_pos );
		out_data = vec4( env( v , 0.0 ) , 1.0 );
		return;
		//discard;
	}
	vec3 pos = posFromZ( vec2( -1.0 + 2.0 * distfragpos.x , -1.0 + 2.0 * distfragpos.y ) , depth , CAM );
	vec4 specw = unpack4i( buf0.w );
	vec3 norm = normFromi( buf0 );
	vec3 diff = unpack4i( buf0.y ).xyz;
	vec3 l = light( pos , norm , CAM.pos , specw ) * mix( vec3( 1.0 ) , diff , specw.x );
	float ao = ssao( BUFFER0 , vec4( norm , depth ) , distfragpos , 0.3 );
	out_data += vec4( WATERK * diff * ao * l , 1.0  );
	//vec4( pow( texture2D( DLIGHT[0].DepthMap_Buffer , frag_pos ).x , 4.0 ) );
}