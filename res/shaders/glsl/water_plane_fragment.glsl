#version 430
$include uniforms.glsl
$include include.glsl
$include pervertex.glsl
in PerVertex pvo;
out vec4 buf;
void main()
{
	vec4 ne = texture2D( RGB_NORMAL_A_HEIGHT_TEXTURE , pvo.texcoord.xy );
	vec4 bump = -1.0 + ne * 2.0;
	vec3 newnormal = normalize( pvo.normal * bump.z-pvo.binormal * bump.y + pvo.tangentnormal * bump.x );// , 
	buf = vec4(  pvo.depth , 0.0 , 0.0 , pack4f( vec4( 0.5 + 0.5 * newnormal.xy , 1.0 , 0.0 ) ) );
} 