#pragma once
#include <openGL\DrawableGL.h>
struct InstanceInfo
{
	float time;
	float last_time;
	float dist_to_cam;
	float selection_id;
	int mixing;
	int cur_anim;
	int last_anim;
	int auto_height;
	f3 pos;
	f3 look;
	f3 left;
	f3 up;
};
class AnimatedInstancedPolyMesh : public PolyMeshGL
{
private:
	TextureHolderGL __textures;
	BoneAnimInTexHolderGL __anim_intex;
	std::unique_ptr< Polymesh > __mesh;
	int _bone_count;
public:
	AnimatedInstancedPolyMesh() = default;
	AnimatedInstancedPolyMesh( AnimatedInstancedPolyMesh && );
	void operator=( AnimatedInstancedPolyMesh && );
	AnimatedInstancedPolyMesh( std::unique_ptr< Polymesh > && );
	void init( std::unique_ptr< Polymesh > && );
	void init();
	void release();
	void bindUniforms( InstanceInfo const & ) const;
	~AnimatedInstancedPolyMesh();
};