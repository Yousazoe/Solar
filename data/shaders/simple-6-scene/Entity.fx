// enum EntityType
static const int MESH_ENTITY = 1;
static const int LIGHT_ENTITY = 2;
static const int SPHERE_ENTITY = 5;
static const int QUAD_ENTITY = 6;

static const int MASK_MESH = 1 << MESH_ENTITY;
static const int MASK_LIGHT = 1 << LIGHT_ENTITY;
static const int MASK_SPHERE = 1 << SPHERE_ENTITY;
static const int MASK_QUAD = 1 << QUAD_ENTITY;

static const uint MASK_ALL = MASK_MESH | MASK_SPHERE | MASK_QUAD | MASK_LIGHT;
static const uint MASK_GEOMETRY = MASK_MESH | MASK_SPHERE | MASK_QUAD;


struct Entity
{
	int type;  
	int index;
};

struct SphereMesh
{
	float3 position;
	float radius;
	int material;
};

struct QuadMesh 
{
	float3 position;
	float3 u;
	float3 v;
	int material;
};

SphereMesh ___SphereMesh(in float3 position, float radius, int material) {
	SphereMesh ball = (SphereMesh)0;
	ball.position = position;
	ball.radius = radius;
	ball.material = material;
	return ball;
}

QuadMesh ___QuadMesh(in float3 position, float3 u, float3 v, int material) {
	QuadMesh quad = (QuadMesh)0;
	quad.position = position;
	quad.u = u;
	quad.v = v;
	quad.material = material;
	return quad;
}
