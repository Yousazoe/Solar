SamplerState _samp_point: register(s0);
cbuffer Frame
{
	uint _frame_index;
	float2 _seed;
}

struct Material { float3 baseColor; };
Material ___Material(float3 baseColor) 
{ 
	Material material; 
	material.baseColor = baseColor; 
	return material;
}

#define MaterialBuffer(xxx) Material xxx[4]


float3 pathtracing(in Ray ray, inout Random random)
{
	// init scene
	Scene scene = (Scene)0;

	MaterialBuffer(materials);
	materials[0] = ___Material(float3(1, 1, 1));
	materials[1] = ___Material(float3(1, 0, 0));
	materials[2] = ___Material(float3(0, 0, 1));
	materials[3] = ___Material(float3(0.5,0.5,0.5));
	int white = 0;
	int red = 1;
	int blue = 2;
	int grey = 3;

	SphereMeshBuffer(balls);
	balls[0] = ___SphereMesh(float3(0, -1.5, 0), 1, white);
	balls[0].material = 0; 
	scene.nball = 1;

	QuadMeshBuffer(quads);
	quads[0] = ___QuadMesh(float3(-2.5f, 2.5f, 2.5f), float3(5, 0, 0), float3(0, -5, 0), grey);  // front 
	quads[1] = ___QuadMesh(float3(-2.5f, 2.5f, -2.5f), float3(0, 0, 5), float3(0, -5, 0), red);  // left
	quads[2] = ___QuadMesh(float3(2.5f, 2.5f, 2.5f), float3(0, 0, -5), float3(0, -5, 0), blue);		// right
	quads[3] = ___QuadMesh(float3(-2.5f, -2.5f, 2.5f), float3(5, 0, 0), float3(0, 0, -5), grey);	// down
	quads[4] = ___QuadMesh(float3(-2.5f, 2.5f, -2.5f), float3(5, 0, 0), float3(0, 0, 5), grey);		// up
	quads[5] = ___QuadMesh(float3(2.5f, 2.5f, -2.5f), float3(-5, 0, 0), float3(0, 0, -5), grey);	// back
	scene.nquad = 6;

	// lights
	LightBuffer(lights);
	lights[0] =
		___QuadLight(
			float3(-1.f, 1.5f, -2.5f), float3(1, 1, 1) * 3.14,
			float3(2, 0, 0), float3(0, 0, 2)
		);
	scene.nlight = 1;


	// first hit
	Hit hit;
	trace_scene(ray, INFINITY, scene, balls, quads, lights, MASK_ALL, hit);
	if (hit.hit)
	{
		if (hit.entity.type == LIGHT_ENTITY) {
			int index = hit.entity.index;
			return lights[index].energy;
		}
		else if (hit.entity.type == SPHERE_ENTITY || hit.entity.type == QUAD_ENTITY) {
			Material material = materials[hit.surface.material];
			return material.baseColor;
		}
	} else {
		return float3(0, 0, 0);
	}

	return 0;
}


float4 pathtrace_loop(
	in Ray ray,
	in float2 uv,
	in uint2 resolution
)
{
	Random random = { uv, _seed };
	float3 color = pathtracing(ray, random);
	return float4(color, 1.0f);
}

