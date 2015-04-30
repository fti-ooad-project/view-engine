#version 430
$include include.glsl
$include light_func.glsl
layout( location = 0 ) uniform usampler2D BUFFER0;//pos cull
layout( location = 1 ) uniform sampler2D ENV;//normal depth
layout( location = 2 ) uniform sampler2D SELECTBUFF;//normal depth
layout( location = 3 ) uniform mat4 VIEWPROJ;
layout( location = 8 ) uniform sampler2D LIGHTK;
layout( location = 12 ) uniform sampler2D WATER_REFLECTBUF;
//layout( location = 3 ) uniform sampler2DArray TEST;//spec gloss
//layout( location = 3 ) uniform sampler2D BUFFER3;//diff emiss
layout( location = 11 ) uniform usampler2D WATER_BUFFER3;//diff emiss
layout( location = 4 ) uniform Camera CAM;
//layout( location = 9 ) uniform int OLIGHT_COUNT;
layout( location = 10 ) uniform int DLIGHT_COUNT;
layout( location = 13 ) uniform int SLIGHT_COUNT;
//layout( location = 20 ) uniform OmniLight OLIGHT[MAX_LIGHTS_CASTER];
layout( location = 40 ) uniform DirectedLight DLIGHT[MAX_LIGHTS_CASTER];
layout( location = 20 ) uniform SimpleLight SLIGHT[MAX_LIGHTS];

//layout( location = 11 ) uniform sampler2D ANIM_TEX;
//specw.r - metallness
//specw.g - fresnel
//specw.a - exponent
in vec2 frag_pos;
out vec3 out_data;
vec3 env( vec3 n , float s )
{
	float alpha = atan( n.y , n.x );
	float theta = atan( n.z , length( n.xy ) );
	//return texture( ENV , vec2( 0.5 + alpha / pi * 0.5 , 0.5 - theta / pi ) ).xyz;
	//float mipmapLevel = textureQueryLod( ENV , textureCoord ).x;
	//if( -dot( n , DLIGHT[0].dir ) > 0.999 )
	//		return vec3( 1.1 );
	return //vec3( pow( max( 0.0 , min( 1.0 , dot( n , -DLIGHT[0].dir ) * 1.001 ) ) , ( 1.0 - s ) * 15.0 ) ) + 
	textureLod( ENV , vec2( 0.5 + alpha / pi * 0.5 , 0.5 - theta / pi ) , s * 15.0 ).xyz;
}
float diffuse( vec3 n , vec3 l , float ldist2 , float lr )
{
	return max( 0.0 , dot( n , l ) ) / ldist2 * 0.25 / pi;
}
float specular( vec3 n , vec3 l , vec3 refl , float spec , float ldist2 , float lr )
{
	float cosa = sqrt( ldist2 / ( ldist2 + lr * lr ) );
	float func;
	float cosx = max( 0.0 , dot( refl , l ) );
	if( cosx < cosa )
		func = pow( cosx / cosa , max( 1.0 , pow( spec , 5.0 ) * 40.0 ) );
	else
		func = 1.0;
	return max( 0.0 , dot( n , l ) ) * 0.5 *
	func / max( 0.01 , ldist2 - lr * lr ) * ( spec * 40.0 + 1.0 ) * 0.03;
}
vec3 light( vec3 p , vec3 n , vec3 v , vec4 specw , vec3 albedo , float env_k )
{
	//vec3 diff_out = vec3( 0.0 );
	//vec3 spec_out = vec3( 0.0 );
	vec3 refl = reflect( normalize( p - v ) , n );
	vec3 view = normalize( p - v );
	float freshnel = mix( 1.0 , max( 0.0 , 1.0 - specw.g * dot( -normalize( p - v ) , n ) ) , specw.g );
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
	vec3 out_c = vec3( 0.0 );
	for( int i = 0; i < SLIGHT_COUNT; i++ )//COUNT!!!
	{
		vec3 rl = SLIGHT[i].pos_r.xyz - p;
		float ldist2 = dot( rl , rl );
		vec3 l = normalize( rl );
		float sk = specular( n , l , refl , specw.w , ldist2 , SLIGHT[i].pos_r.w );
		out_c += freshnel * sk * SLIGHT[i].colori.xyz * SLIGHT[i].colori.w;
	}
	for( int i = 0; i < DLIGHT_COUNT; i++ )//COUNT!!!
	{
		float sk = specular( n , -DLIGHT[i].dir , refl , specw.w , 1.0 , 0.04 );
		out_c +=  
		freshnel * sk * DLIGHT[i].colori.xyz * DLIGHT[i].colori.w * mix( smoothLightDir( p , n , 0.1 , DLIGHT[i].toLightViewProj , DLIGHT[i].DepthMap_Buffer ) , env_k , specw.w );
	}
	return out_c + ( env_k * 0.7 + 0.3 ) * freshnel * env( refl , 1.0 - specw.w ) * albedo;
	//return vec3( smoothLightDir( p , n , 0.1 , DLIGHT[0].toLightViewProj , DLIGHT[0].DepthMap_Buffer ) );
}
vec3 traceLights( vec3 p , vec3 v , float depth )
{
	for( int i = 0; i < SLIGHT_COUNT; i++ )//COUNT!!!
	{
		vec3 dp = p - SLIGHT[i].pos_r.xyz;
		if( dot( dp , v) > 0.0 ) return vec3( 0.0 );
		float a = dot( v , dp );
		float d = a * a - dot( dp , dp ) + SLIGHT[i].pos_r.w * SLIGHT[i].pos_r.w;
		if( d >= 0.0 && length( dp ) < depth )
			return SLIGHT[i].colori.xyz;// * SLIGHT[i].colori.w;
	}
	return vec3( 0.0 );
}
vec3 lightFog( vec3 p )
{
	vec3 lightness = vec3( 0.0 );
	for( int i = 0; i < 1; i++ )//COUNT!!!
	{
		lightness += DLIGHT[i].colori.xyz * DLIGHT[i].colori.w
		* smoothLightDir( p , vec3( 0.0 , 0.0 , 0.0 ) , 0.1 , DLIGHT[i].toLightViewProj , DLIGHT[i].DepthMap_Buffer );
	}
	return lightness.xyz;
}
vec3 fog()
{
	const int COUNT = 100;
	const float dr = 1.0;
	vec3 out_c = vec3( 0.0 );
	for( int i = 0; i < COUNT; i++ )
	{
		vec3 p = camRay( CAM , frag_pos ) * dr * ( i + 1 ) + CAM.pos;
		out_c += lightFog( p );
	}
	return out_c / COUNT * 0.5 * vec3( 0.5 , 0.6 , 0.7 );
}
float reflectWater( float start_depth , vec2 start_tex , vec3 wnorm , vec3 wpos )
{
	//float out_c = 1.0;
	const int COUNT = 40;
	const float DIST = 50.0;
	const float DR = DIST / COUNT;
	vec3 dest_pos = wpos + DIST * reflect( normalize( wpos - CAM.pos ) , wnorm );
	vec4 dest_camspace = VIEWPROJ * vec4( dest_pos , 1.0 );
	float dest_depth = dest_camspace.z;
	if( dest_depth < start_depth )
		return 1.0;
	float DDEPTH = ( dest_depth - start_depth ) / COUNT;
	dest_camspace /= dest_camspace.w;
	vec2 dest_tex = 0.5 + 0.5 * dest_camspace.xy;
	vec2 DTEX = ( dest_tex - start_tex ) / COUNT;
	for( int i = 1; i < COUNT + 1; i++ )
	{
		float idepth = start_depth + i * DDEPTH;
		vec2 itex = start_tex + i * DTEX;
		uvec4 imgbuf = texture( BUFFER0 , itex );
		float imgdepth = depthFromi( imgbuf );
		vec4 wY = unpack4i( imgbuf.y );
		if( wY.w > 0.0 && imgdepth < idepth && imgdepth > start_depth )
		{
			return 0.5;//float( i ) / COUNT;
		}
	}
	return 1.0;
}
void main()
{
	//out_data = uvec3( texture( WATER_BUFFER3 , vec2( frag_pos ) ).xyz ); 
	//out_data = vec3( texture( WATER_REFLECTBUF , frag_pos ).x < 1.0 ? 0.0 : 1.0 ); return;
	out_data = vec3( 0.0 );
	definePoisson();
	
	
	vec2 distfragpos = frag_pos;
	///
	uvec4 wbuf = texture( WATER_BUFFER3 , frag_pos );
	vec4 wY = unpack4i( wbuf.y );
	vec3 WATERK = vec3( 1.0 );
	float wdepth = 1000.0;
	vec2 screenspacewaternorm = vec2( 0.0 );
	float DDEPTH = 0.0;
	if( wY.w > 0.0 )
	{
		//screenspacewaternorm = 2.0 * wY.xy - 1.0;
		//distfragpos += -screenspacewaternorm * 0.05;// * min( DDEPTH / 3.0 , 1.0 );
		
		uvec4 buf0 = texture( BUFFER0 , frag_pos );
		float depth = depthFromi( buf0 );
		
		wdepth = depthFromi( wbuf );
		vec3 wnorm = normFromi( wbuf );
		if( frag_pos.x > 0.01 && frag_pos.x < 0.99 && frag_pos.y > 0.01 && frag_pos.y < 0.99 ) distfragpos += -wnorm.xy * 0.05;
		vec3 wpos = vec3( wnorm.xy , 0.0 ) * 0.5 + posFromZ( vec2( -1.0 + 2.0 * frag_pos.x , -1.0 + 2.0 * frag_pos.y ) , wdepth , CAM );
		DDEPTH = abs( wdepth - depth );
		float dw = max( 0.0 , dot( normalize( CAM.pos - wpos ) , wnorm ) );
		WATERK = dw * mix( vec3( 0.7 , 0.8 , 0.9 ) , vec3( 0.1 , 0.15 , 0.2 ) , min( 1.0 , DDEPTH / 3.0 ) );
		vec3 wl = light( wpos , wnorm , CAM.pos , vec4( 0.0 , 0.5 , 0.0 , 0.99 ) , vec3( 0.9 , 0.85 , 0.89 ) , ( texture( WATER_REFLECTBUF , distfragpos ).x < 1.0 ? 0.0 : 1.0 ) );
		out_data += ( 1.0 - dw ) * 
		vec3( wl );//vec4( dot( wnorm , vec3( 0.0 , 0.0 , 1.0 ) ) );reflectWater( wdepth , frag_pos , wnorm , wpos ) * 
		
		
	}
	uvec4 buf0 = texture( BUFFER0 , distfragpos );
	float depth = depthFromi( buf0 );
	///
	/*if( frag_pos.x > 0.03 && frag_pos.x < 0.97 && frag_pos.y > 0.03 && frag_pos.y < 0.97 )
	{
		distfragpos += screenspacewaternorm * 0.01 * ( wdepth - depth );
		buf0 = texture( BUFFER0 , distfragpos );
		depth = depthFromi( buf0 );
	}*/
	//out_data += vec4( fog() , 1.0 );
	const vec3 FOG = vec3( 0.1 , 0.2 , 0.2 );
	vec3 v = camRay( CAM , frag_pos );
	out_data += traceLights( CAM.pos , v , depth );
	if( buf0.x == 0 )
	{
		
		if( -dot( v , DLIGHT[0].dir ) > 0.999 )
			out_data = vec3( 1.5 );
		else
		//	out_data = vec3( 0.8 );
			out_data += mix( env( v , 0.0 ) , FOG , 1.0 - abs( v.z ) );
		//return;
	}else
	{
		vec3 pos = posFromZ( vec2( -1.0 + 2.0 * distfragpos.x , -1.0 + 2.0 * distfragpos.y ) , depth , CAM );
		vec4 specw = unpack4i( buf0.w );
		vec3 norm = normFromi( buf0 );
		vec4 diff = unpack4i( buf0.y );
		vec3 l = light( pos , norm , CAM.pos , specw , diff.xyz , 1.0 );// * mix( vec3( 1.0 ) , diff.xyz , specw.x );
		//float ao = 1.0;//ssao( BUFFER0 , vec4( norm , depth ) , distfragpos , 0.8 );
		out_data += WATERK * l;
		out_data = mix( out_data , FOG , 1.0 - exp( - min( depth , wdepth ) * 0.01 ) );
	}
	
	vec4 selbuf = texture( SELECTBUFF , frag_pos );
	out_data += selbuf.xyz;// + traceLights( CAM.pos , v );
	//vec3( pow( texture2D( DLIGHT[0].DepthMap_Buffer , frag_pos ).x , 4.0 ) );
}