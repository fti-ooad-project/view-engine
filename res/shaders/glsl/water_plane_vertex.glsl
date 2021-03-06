#version 430
$include include.glsl
$include uniforms.glsl
$include attributes.glsl
$include pervertex.glsl
out PerVertex pvo;
void main()
{
	vec4 p;
	switch( PASSID )
	{
	case 0:
		p = vec4( WATER_POS_SIZE.xyz + invertex_position * WATER_POS_SIZE.w , 1.0 );
	break;
	case 1:
		p = vec4( vec3( 0.0 , 0.0 , WATER_POS_SIZE.z - 0.1 ) + invertex_position * 500.0 , 1.0 );
	break;
	}
	
	pvo.binormal = invertex_binormal ;
	pvo.tangentnormal = invertex_tangentnormal;
	pvo.texcoord = vec2( 0.5 - 0.5 * invertex_position.x , 0.5 + 0.5 * invertex_position.y );
	pvo.normal = invertex_normal;
    vec4 pos = MAT4X4_VIEWPROJ[0] * vec4( p.xyz , 1.0 );
	pvo.position = p.xyz;
	pvo.depth = pos.z;
    gl_Position = pos;
}
