layout( location = 0 ) uniform int FLAGS;
layout( location = 1 ) uniform sampler2D  RGB_NORMAL_A_HEIGHT_TEXTURE;
layout( location = 2 ) uniform sampler2D  RGB_DIFFUSE_A_OPACITY_TEXTURE;
layout( location = 3 ) uniform sampler2D  RGB_SPECULAR_A_GLOSSNESS_TEXTURE;

layout( location = 6 ) uniform mat4 MAT4X4_MODEL;

layout( location = 7 ) uniform float TIME;
layout( location = 8 ) uniform int MIXING;
layout( location = 9 ) uniform float LAST_TIME;
layout( location = 10 ) uniform int BONE_COUNT;

layout( location = 11 ) uniform vec3 CAM_POS;
layout( location = 12 ) uniform int TESSELATE;

layout( location = 20 ) uniform sampler2D ANIM_TEX;
layout( location = 21 ) uniform sampler2D LAST_ANIM_TEX;

layout( location = 30 ) uniform mat4 MAT4X4_VIEWPROJ[6];