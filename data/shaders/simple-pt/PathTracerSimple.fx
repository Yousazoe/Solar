SamplerState _samp_point: register(s0);

Texture2D t_prev_result;
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



inline float3 uniform_sample_sphere(float2 u)
{
	float y = 1.0f - 2.0f * u.x;
	float r = sqrt(max(0.f, 1.0f - y * y));
	float phi = 2.0f * PI * u.y;
	float x = r * cos(phi);
	float z = r * sin(phi);

	return float3(x, y, z);
}

inline float3 uniform_sample_hemisphere(float2 u)
{
	// u = (cosTheta) ^2  
	// u : [0..1] 
	float sinTheta = sqrt(1 - u.x * u.x);
	float phi = 2 * PI * u.y;
	float x = sinTheta * cos(phi);
	float z = sinTheta * sin(phi);

	return normalize(float3(x, u.x, z));
}


inline float3 importance_sample_cosweight(float2 u)
{
	// u.x == sqr(sinTheta)
	float sinTheta = sqrt(u.x);
	float phi = 2.0 * PI * u.y;
	return float3(cos(phi) * sinTheta, sqrt(max(0.0, 1.0 - u.x)), sin(phi) * sinTheta);
}

struct Bounce
{
	uint d;
	float3 throughput;
	Ray ray;
	float pdf;
};

void bounce_out(
	in const Surface surface,
	in const Material material,
	inout Bounce bounce, 
	inout Random random
)
{
	// 反弹方向， 在 tangent 空间
	float3x3 tbn = get_tangent_space(surface.normal);
	float2 u = rand2(random);
	float3 brdf_dir = importance_sample_cosweight(u);
	// 把向量转换到世界空间
	brdf_dir = tangent_to_world(brdf_dir, tbn);
	// 矫正到正半球上
	brdf_dir *= sign(dot(surface.normal, brdf_dir));

	// 全局光照方程 积分 : radiance * ( brdf(V,L) * dot(L,N) / pdf ) 
	float3 brdf = to_linear(material.baseColor) * INV_PI; // lambert 材质的 brdf = albedo / PI 
	float cosLi = dot(brdf_dir, surface.normal);
	float pdf = cosLi * INV_PI;
	if (cosLi > 0)
	{
		bounce.ray.direction = brdf_dir;
		bounce.ray.origin = surface.position + brdf_dir * EPS; // avoid self occlussion
		bounce.pdf = pdf;
		bounce.throughput *= brdf * cosLi / pdf;
	}
}


float3 pathtracing(in Ray ray, inout Random random)
{
	
	
	// init scene
	Scene scene = (Scene)0;

	MaterialBuffer(materials);
	materials[0] = ___Material(float3(1, 1, 1));
	materials[1] = ___Material(float3(1, 0, 0));
	materials[2] = ___Material(float3(0, 0, 1));
	int white = 0;
	int red = 1;
	int blue = 2;

	SphereMeshBuffer(balls);
	balls[0] = ___SphereMesh(float3(0, -1.5, 0), 1, white);
	balls[0].material = 0; 
	scene.nball = 1;

	QuadMeshBuffer(quads);
	quads[0] = ___QuadMesh(float3(-2.5f, 2.5f, 2.5f), float3(5, 0, 0), float3(0, -5, 0), white);  // front 
	quads[1] = ___QuadMesh(float3(-2.5f, 2.5f, -2.5f), float3(0, 0, 5), float3(0, -5, 0), red);  // left
	quads[2] = ___QuadMesh(float3(2.5f, 2.5f, 2.5f), float3(0, 0, -5), float3(0, -5, 0), blue);		// right
	quads[3] = ___QuadMesh(float3(-2.5f, -2.5f, 2.5f), float3(5, 0, 0), float3(0, 0, -5), white);	// down
	quads[4] = ___QuadMesh(float3(-2.5f, 2.5f, -2.5f), float3(5, 0, 0), float3(0, 0, 5), white);		// up
	quads[5] = ___QuadMesh(float3(2.5f, 2.5f, -2.5f), float3(-5, 0, 0), float3(0, 0, -5), white);	// back
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
	} else {
		return float3(0, 0, 1);
	}

	float3 radiance = 0;
	int nbounce = 4;
	Bounce bounce = { 0, float3(1.f, 1.f, 1.f), ray, 1.f };
	for (int i = 0; i < nbounce; ++i)
	{
		Material mat = materials[hit.surface.material];
		bounce_out(hit.surface, mat, bounce, random);
		hit = NotHit();
		trace_scene(bounce.ray, INFINITY, scene, balls, quads, lights, MASK_ALL, hit);
		if (hit.hit)
		{
			if (hit.entity.type == LIGHT_ENTITY)
			{
				Light light = lights[hit.entity.index];
				radiance += light.energy * bounce.throughput;
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




float4 pathtrace_loop(
	in Ray ray,
	in float2 uv,
	in uint2 resolution
)
{
	Random random = { uv, _seed };
	float3 color = pathtracing(ray, random);
	float3 prev = t_prev_result.Sample(_samp_point, uv).rgb;
	color = (prev * _frame_index + color) / (_frame_index + 1);

	return float4(color, 1.0f);
}

