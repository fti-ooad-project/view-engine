#pragma once
/*layout( location = 0 ) uniform int FLAGS;
layout( location = 1 ) uniform sampler2D  RGB_NORMAL_A_HEIGHT_TEXTURE;
layout( location = 2 ) uniform sampler2D  RGB_DIFFUSE_A_OPACITY_TEXTURE;
layout( location = 3 ) uniform sampler2D  RGB_SPECULAR_A_GLOSSNESS_TEXTURE;

layout( location = 6 ) uniform mat4 MAT4X4_MODEL;

layout( location = 7 ) uniform float TIME;
layout( location = 8 ) uniform int MIXING;
layout( location = 9 ) uniform float LAST_TIME;
layout( location = 10 ) uniform int BONE_COUNT;
layout( location = 13 ) uniform int CUR_ANIM;
layout( location = 14 ) uniform int LAST_ANIM;
layout( location = 15 ) uniform int INSTANSING;
layout( location = 17 ) uniform int LIGHTPASS;

layout( location = 11 ) uniform vec3 CAM_POS;
layout( location = 16 ) uniform float DIST_TO_CAM;

layout( location = 20 ) uniform sampler2D ANIM_TEX[ 10 ];

layout( location = 30 ) uniform mat4 MAT4X4_VIEWPROJ[ 6 ];

layout( location = 0 ) uniform usampler2D BUFFER0;//pos cull
layout( location = 1 ) uniform sampler2D ENV;//normal depth
//layout( location = 2 ) uniform sampler2D BUFFER2;//spec gloss
//layout( location = 3 ) uniform sampler2D BUFFER3;//diff emiss
//layout( location = 11 ) uniform sampler2D WATER_BUFFER3;//diff emiss
layout( location = 4 ) uniform Camera CAM;
layout( location = 9 ) uniform int OLIGHT_COUNT;
layout( location = 10 ) uniform int DLIGHT_COUNT;
layout( location = 20 ) uniform OmniLight OLIGHT[MAX_LIGHTS_CASTER];
layout( location = 32 ) uniform DirectedLight DLIGHT[MAX_LIGHTS_CASTER];
*/
#define FLAGS 0
#define ARRAY_TEXTURE 1
#define TIME 7
#define BONE_COUNT 10
#define PASSID 17
#define PASS_NORMAL 0
#define PASS_LIGHT 1
#define PASS_WATER 2
#define PASS_REFLECT 3
#define WATER_Z 18
#define CAM_POS 11
#define ANIM_TEX 20
#define INSTANSING 15
#define MAT4X4_VIEWPROJ 30
#define BUFFER0 0
#define ENV 1
#define CAM 4
//#define OLIGHT_COUNT 9
#define DLIGHT_COUNT 10
//#define OLIGHT 20
#define DLIGHT 40

