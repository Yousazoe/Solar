

struct Surface
{
	float3 position;
	int material;   // material
	float3 normal;
	float3 ffnormal;
};

struct Hit
{
	Entity entity;
	float hit_dist;
	bool hit;
	Surface surface;
};

Hit NotHit()
{
	static Surface surface = (Surface)0;
	static Hit not_hit = { { 0, -1 }, INFINITY, false, surface };
	return not_hit;
}