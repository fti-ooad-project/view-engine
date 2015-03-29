layout( location = 0 ) uniform int FLAGS;
layout( location = 1 ) uniform sampler2DArray  TEXTURE_ARRAY;
layout( location = 2 ) uniform sampler2DArray ANIM_TEX_ARRAY;
layout( location = 3 ) uniform sampler2D BUFFER1;

layout( location = 6 ) uniform mat4 MAT4X4_MODEL;

layout( location = 7 ) uniform float TIME;
layout( location = 8 ) uniform int MIXING;
layout( location = 9 ) uniform float LAST_TIME;
layout( location = 10 ) uniform int BONE_COUNT;
layout( location = 13 ) uniform int CUR_ANIM;
layout( location = 14 ) uniform int LAST_ANIM;
layout( location = 15 ) uniform int INSTANSING;
layout( location = 17 ) uniform int PASSID;
layout( location = 18 ) uniform float WATERZ;
#define PASS_NORMAL 0
#define PASS_LIGHT 1
#define PASS_WATER 2
#define PASS_REFLECT 3
layout( location = 11 ) uniform vec3 CAM_POS;
layout( location = 16 ) uniform float DIST_TO_CAM;



layout( location = 30 ) uniform mat4 MAT4X4_VIEWPROJ[6];