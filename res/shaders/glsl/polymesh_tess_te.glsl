#version 430
layout( triangles , equal_spacing , ccw ) in;
$include pervertex.glsl
$include uniforms.glsl
in PerVertex pvtc[];
out PerVertex pvte;
float interpolate1D( float v0 , float v1 , float v2 )
{
   	return gl_TessCoord.x * v0 + gl_TessCoord.y * v1 + gl_TessCoord.z * v2;
}
vec2 interpolate2D( vec2 v0 , vec2 v1 , vec2 v2 )
{
   	return vec2(gl_TessCoord.x ) * v0 + vec2( gl_TessCoord.y ) * v1 + vec2( gl_TessCoord.z ) * v2;
}
vec3 interpolate3D( vec3 v0 , vec3 v1 , vec3 v2 )
{
   	return vec3( gl_TessCoord.x ) * v0 + vec3( gl_TessCoord.y ) * v1 + vec3( gl_TessCoord.z ) * v2;
}
vec4 interpolate4D( vec4 v0 , vec4 v1 , vec4 v2 )
{
   	return vec4( gl_TessCoord.x ) * v0 + vec4( gl_TessCoord.y ) * v1 + vec4( gl_TessCoord.z ) * v2;
}
float bez()
{
	const float p = 1.0;
	return 0.9 * pow( 1.0 - gl_TessCoord.x , p ) *
			pow( 1.0 - gl_TessCoord.y , p ) *
			pow( 1.0 - gl_TessCoord.z , p );	
}
void main()
{
	pvte.binormal = interpolate3D( pvtc[0].binormal , pvtc[1].binormal , pvtc[2].binormal );
	pvte.tangentnormal = interpolate3D( pvtc[0].tangentnormal , pvtc[1].tangentnormal , pvtc[2].tangentnormal );
	pvte.texcoord = interpolate2D( pvtc[0].texcoord , pvtc[1].texcoord , pvtc[2].texcoord );
	pvte.select_id = interpolate1D( pvtc[0].select_id , pvtc[1].select_id , pvtc[2].select_id );
	pvte.dist_to_cam = pvtc[0].dist_to_cam;
	vec4 p = interpolate4D( gl_in[0].gl_Position , gl_in[1].gl_Position , gl_in[2].gl_Position );
	pvte.normal = interpolate3D( pvtc[0].normal , pvtc[1].normal , pvtc[2].normal );
	vec4 pos = vec4( p.xyz , 1.0 );
	pvte.position = p.xyz;
	//float h = texture2D( RGB_NORMAL_A_HEIGHT_TEXTURE , pvte.texcoord ).w;
	//vec4 npos = MAT4X4_VIEWPROJ[0] * vec4( p.xyz + pvte.normal * 0.08 * ( -0.3 + h ) , 1.0 );
	//pvte.depth = npos.z;
	
	gl_Position = pos;
}