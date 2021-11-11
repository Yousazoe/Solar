struct Scene
{
	uint nball;
	uint nquad;
	uint nlight;
	uint pad;
};

#define SphereMeshBuffer(xxx) SphereMesh xxx[1]
#define QuadMeshBuffer(xxx) QuadMesh xxx[6]
#define LightBuffer(xxx) Light xxx[1]


























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


float trace_light(in const Ray ray, in const Light light)
{
	[branch]
	switch (light.type)
	{
	case QuadLight:
	{
		float3 u = light.u;
		float3 v = light.v;
		float3 normal = normalize(cross(u, v));
		[branch] if (dot(normal, ray.direction) >= 0)
			return INFINITY;
		return ray_rect_intersect(light.position, u, v, normal, ray);
	}
	default:
		break;
	}

	return INFINITY;
}

void trace_scene_lights(
	in const Ray ray, 
	in Scene scene,
	in LightBuffer(lights),
	inout Hit hit
)
{
	float t = hit.hit_dist;
	uint light_count = scene.nlight;
	[loop]
	for (uint i = 0; i < light_count; ++i)
	{
		Light light = lights[i];
		float d = trace_light(ray, light);
		[branch]
		if (d < t)
		{
			t = d;
			hit.entity.index = i;
			hit.entity.type = LIGHT_ENTITY;
			hit.hit_dist = t;
			hit.hit = true;
		}
	}
}

void trace_scene_sphere(
	in const Ray ray, 
	in Scene scene,
	in SphereMeshBuffer(balls),
	inout Hit hit
)
{
	float t = hit.hit_dist;
	[loop]
	for (uint i = 0; i < scene.nball; ++i)
	{
		SphereMesh mesh = balls[i];
		float d = ray_sphere_intersect(mesh.position, mesh.radius, ray);
		[branch] if (d < t)
		{
			t = d;
			hit.entity.index = i;
			hit.entity.type = SPHERE_ENTITY;
			hit.surface.position = ray.origin + ray.direction * d;
			hit.surface.normal = normalize(hit.surface.position - mesh.position);
			hit.surface.ffnormal = dot(hit.surface.normal, ray.direction) <= 0.0 ? hit.surface.normal : hit.surface.normal * -1.0;
			hit.surface.material = mesh.material;
		}
	}

	[branch]
	if (t < hit.hit_dist) {
		hit.hit_dist = t;
		hit.hit = true;
	}
}

void trace_scene_quad(
	in const Ray ray,
	in Scene scene,
	in QuadMeshBuffer(quads),
	inout Hit hit
)
{
	float t = hit.hit_dist;
	[loop]
	for (uint i = 0; i < scene.nquad; ++i)
	{
		QuadMesh mesh = quads[i];
		float3 u = mesh.u;
		float3 v = mesh.v;
		float3 normal = normalize(cross(u, v));
		[branch]
		if (dot(normal, ray.direction) < 0)
		{
			float d = ray_rect_intersect(mesh.position, u, v, normal, ray);
			[branch]
			if (d < t)
			{
				t = d;
				hit.entity.index = i;
				hit.entity.type = QUAD_ENTITY;
				hit.surface.position = ray.origin + ray.direction * d;
				hit.surface.normal = normal;
				hit.surface.ffnormal = dot(hit.surface.normal, ray.direction) <= 0.0 ? hit.surface.normal : hit.surface.normal * -1.0;
				hit.surface.material = mesh.material;
				hit.hit_dist = d;
				hit.hit = true;
			}
		}
	}
}




void trace_scene(
	in const Ray ray,
	in const float max_dist, // search distance
	in Scene scene, 
	in SphereMeshBuffer(balls), in QuadMeshBuffer(quads),
	in LightBuffer(lights),
	in const uint mask,
	out Hit hit
)
{
	hit = NotHit();
	hit.hit_dist = max_dist;
 
	[branch] if ((mask & MASK_SPHERE) > 0) { trace_scene_sphere(ray, scene, balls, hit); }
	[branch] if ((mask & MASK_QUAD) > 0) { trace_scene_quad(ray, scene, quads, hit); }
	[branch] if ((mask & MASK_LIGHT) > 0) { trace_scene_lights(ray, scene, lights, hit); }
}
