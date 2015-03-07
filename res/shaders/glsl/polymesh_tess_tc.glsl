#version 430
$include uniforms.glsl
$include include.glsl
layout( vertices = 3 ) out;
$include pervertex.glsl
in PerVertex pv[];
out PerVertex pvtc[];
#define ID gl_InvocationID
float getTessLevel( float dist )
{
	if( dist < 1.0 )
		return 4.0;
	return 4.0 / dist;
}
void main()
{
	pvtc[ID].normal = pv[ID].normal;
	pvtc[ID].binormal = pv[ID].binormal;
	pvtc[ID].tangentnormal = pv[ID].tangentnormal;
	pvtc[ID].texcoord = pv[ID].texcoord;
	pvtc[ID].dist_to_cam = pv[ID].dist_to_cam;
	gl_out[ID].gl_Position = gl_in[ID].gl_Position;
	if( ID == 0 )
	{
		vec3 p = ( gl_in[0].gl_Position.xyz + gl_in[1].gl_Position.xyz + gl_in[2].gl_Position.xyz ) * 0.333333;
		vec3 v = -normalize( p - CAM_POS );
		vec3 n = ( pv[0].normal + pv[1].normal + pv[2].normal ) * 0.333333;
		float k = 0.6;//0.3 * pow( 1.0 + dot( v , n ) , 2.0 );
		float dist = k * distance( p , CAM_POS );
		gl_TessLevelOuter[0] = getTessLevel( dist );
		gl_TessLevelOuter[1] = gl_TessLevelOuter[0];
		gl_TessLevelOuter[2] = gl_TessLevelOuter[0];
		gl_TessLevelInner[0] = gl_TessLevelOuter[0];
	}
}