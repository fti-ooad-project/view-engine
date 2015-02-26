layout( location = 10 ) uniform float RADIUS;
layout( location = 11 ) uniform int COUNT;
float fade( float dr )
{
    return 1.0 - dr * dr;
}
vec4 blur( vec2 tx , vec2 v , sampler2D tex )
{
	const float STEP = RADIUS / float( COUNT );
	vec4 outc = vec4( 0.0 );
    float weight = 0.0;
    for( int i = 0; i < 2 * COUNT; i++ )
    {
        float dr = STEP * ( COUNT - i );
        float w = fade( dr / RADIUS );
		vec2 txt = tx;
		txt += v * dr;
        outc += texture2D( tex , txt ) * w;
        weight += w;
    }
    return outc / weight;
}