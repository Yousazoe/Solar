#define MaterialBuffer(xxx) StructuredBuffer<Material> xxx


cbuffer Frame
{
	uint _frame_index;
	float2 _seed;
	int _nbounce;
	Scene _scene;
}

void bounce_out(
	in const Surface surface,
	in const Material material,
	inout Bounce bounce,
	inout Random random
)
{
	float3 N = surface.ffnormal;
	float3 V = -bounce.ray.direction;
	BsdfSample bsdf_sample;
	sample_bxdf(surface, material, V, bsdf_sample, random);
	[branch]
	if (bsdf_sample.pdf > 0)
	{
		float cosLi = abs(dot(bsdf_sample.bsdf_dir, N));
		bounce.ray.direction = bsdf_sample.bsdf_dir;
		bounce.ray.origin = surface.position + bsdf_sample.bsdf_dir * EPS; // avoid self occlussion
		bounce.pdf = bsdf_sample.pdf;
		bounce.throughput *= bsdf_sample.bsdf * cosLi / bsdf_sample.pdf;
	}
	else {
		bounce.throughput = 0;
	}
}


void fetch_material(in MaterialBuffer(materials), inout Surface surface, out Material material)
{
	material = materials[surface.material];
}

float3 pathtracing(
	in Ray ray, 
	in Scene scene,
	in SphereMeshBuffer(balls), in QuadMeshBuffer(quads),
	in LightBuffer(lights),
	in MaterialBuffer(materials),
	inout Random random
)
{
	Hit hit;
	trace_scene(ray, INFINITY, scene, balls, quads, lights, MASK_ALL, hit);
	[branch]
	if (hit.hit)
	{
		[branch]
		if (hit.entity.type == LIGHT_ENTITY) {
			int index = hit.entity.index;
			return lights[index].energy;
		}
	} else {
		return float3(0, 0, 0);
	}

	float3 radiance = 0;
	Bounce bounce = { 0, float3(1.f, 1.f, 1.f), ray, 1.f, float3(0,0,0)  };
	Material material;

	[loop]
	for (int i = 0; i < _nbounce; ++i)
	{
		fetch_material(materials, hit.surface, material);

		// shoot a bsdf sample
		bounce_out(hit.surface, material, bounce, random);

		
		[branch] if (dot(bounce.throughput, bounce.throughput) < EPS) {
			break;
		}

		hit = NotHit();
		trace_scene(bounce.ray, INFINITY, scene, balls, quads, lights, MASK_ALL, hit);
		[branch]
		if (hit.hit)
		{
			[branch]
			if (hit.entity.type == LIGHT_ENTITY)
			{
				Light light = lights[hit.entity.index];
				radiance += evaluate_light(bounce, hit, light) * bounce.throughput;
				break;
			}
		}
		else
		{
					break;
		}
	}
	return radiance;
}

Texture2D t_prev_result;

MaterialBuffer(_materials);
SphereMeshBuffer(_balls); 
QuadMeshBuffer(_quads);
LightBuffer(_lights);

float4 pathtracer_demo(
	in Ray ray, 
	in float2 uv,
	in uint2 resolution
)
{
	uint2 texel = uint2(uv * resolution);
	Random random = { uv, _seed };
	Scene scene = _scene;

	
	float3 color = pathtracing(ray, scene, _balls, _quads, _lights, _materials, random);
	
	float3 prev = t_prev_result.Sample(_samp_point, uv).rgb;
	color = (prev * _frame_index + color) / (_frame_index + 1);

	return float4(color, 1.0f);
}

