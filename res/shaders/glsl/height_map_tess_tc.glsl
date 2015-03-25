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
	dist *= 0.5;
	//if( dist < 50.0 )
	//	return 20.0;
	return min( 20.0 , max( 1.0 , 50.0 * exp( -dist * 0.02 ) ) );
}
void main()
{
	pvtc[ID].normal = pv[ID].normal;
	/*pvtc[ID].binormal = pv[ID].binormal;
	pvtc[ID].tangentnormal = pv[ID].tangentnormal;*/
	pvtc[ID].texcoord = pv[ID].texcoord;
	//pvtc[ID].dist_to_cam = pv[ID].dist_to_cam;
	gl_out[ID].gl_Position = gl_in[ID].gl_Position;
	if( ID == 0 )
	{
		float dist0 = distance( gl_in[0].gl_Position.xyz , CAM_POS );
		float dist1 = distance( gl_in[1].gl_Position.xyz , CAM_POS );
		float dist2 = distance( gl_in[2].gl_Position.xyz , CAM_POS );
		gl_TessLevelOuter[0] = getTessLevel( dist1 + dist2 );
		gl_TessLevelOuter[1] = getTessLevel( dist0 + dist2 );
		gl_TessLevelOuter[2] = getTessLevel( dist1 + dist0 );
		gl_TessLevelInner[0] = getTessLevel( ( dist1 + dist0 + dist2 ) * 0.666 );
	}
}