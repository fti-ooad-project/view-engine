layout( location = 0 ) in vec3 invertex_position;
layout( location = 1 ) in vec2 invertex_texcoord;
layout( location = 2 ) in vec3 invertex_normal;
layout( location = 3 ) in vec3 invertex_binormal;
layout( location = 4 ) in vec3 invertex_tangentnormal;
layout( location = 5 ) in vec3 invertex_bone_weight;
layout( location = 6 ) in vec4 invertex_indx;

layout( location = 7 ) in vec4 INS_TIME;
layout( location = 8 ) in ivec4 INS_MIXING_ANIM_INDX;
layout( location = 9 ) in vec3 INS_POS;
layout( location = 10 ) in vec3 INS_LOOK;
layout( location = 11 ) in vec3 INS_LEFT;
layout( location = 12 ) in vec3 INS_UP;