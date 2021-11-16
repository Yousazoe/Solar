
struct Scene
{
	uint nball;
	uint nquad;
	uint nlight;
	uint pad;
};

#define SphereMeshBuffer(xxx) StructuredBuffer<SphereMesh> xxx
#define QuadMeshBuffer(xxx) StructuredBuffer<QuadMesh> xxx
#define LightBuffer(xxx) StructuredBuffer<Light> xxx
