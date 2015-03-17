#version 430
layout( location = 0 ) uniform sampler2D RGB_Buffer;
layout( location = 1 ) uniform vec2 V;
layout( location = 2 ) uniform float RADIUS;
layout( location = 3 ) uniform int COUNT;

$include blur_func.glsl
in vec2 frag_pos;
out vec4 out_color;
void main()
{
    out_color = blur( frag_pos , V , RGB_Buffer );
}