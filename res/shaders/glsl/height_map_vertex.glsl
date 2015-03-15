#version 430
$include uniforms.glsl
layout( location = 0 ) in vec2 invertex_position;
$include pervertex.glsl
out PerVertex pv;
layout( location = 4 ) uniform vec4 pos_size;
void main()
{
	/*if( bool( FLAGS & MASK_TEXTURED_NOR ) )
	{
		pv.binormal = normalize( ( m * vec4( invertex_binormal , 0.0 ) ).xyz );
		pv.tangentnormal = normalize( ( m * vec4( invertex_tangentnormal , 0.0 ) ).xyz );
	}
	if( bool( FLAGS & MASK_TEXTURED ) )
		pv.texcoord = invertex_texcoord;
	pv.normal = normalize( ( m * vec4( invertex_normal , 0.0 ) ).xyz );*/
	float h = texture2D( RGB_NORMAL_A_HEIGHT_TEXTURE , 0.5 + 0.5 * invertex_position.xy ).x;
	vec4 p = vec4( vec3( pos_size.xy , 0.0 ) + pos_size.zw * invertex_position , ( h - 0.5 ) * 50.0 , 1.0 );
	//pvo.normal = vec3( 0.0 , 0.0 , 1.0 );
	//vec4 pos = MAT4X4_VIEWPROJ[0] * vec4( p.xyz , 1.0 );
	//pvo.depth = pos.z;
	pv.texcoord = 0.5 + 0.5 * invertex_position;
    gl_Position = p;
} 
