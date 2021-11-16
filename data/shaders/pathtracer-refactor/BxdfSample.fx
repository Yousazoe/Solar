struct BsdfSample
{
	float3 bsdf_dir;
	float3 bsdf;
	float pdf;
};

float3 importance_sample_cosweight(float2 u)
{
	// u == sqr(sinTheta)
	float sinTheta = sqrt(u.x);
	float phi = 2.0 * PI * u.y;
	return float3(cos(phi) * sinTheta, sqrt(max(0.0, 1.0 - u.x)), sin(phi) * sinTheta);
}

void evaluate_diffuse(
	in const Material material,
	float3 L, float3 V, float3 N, 
	out float3 bsdf, out float pdf
)
{
	float NdotL = abs(dot(N, L));
	bsdf = mon2lin(material.baseColor) * INV_PI;
	pdf = NdotL * INV_PI;
}

void sample_bxdf(
	in const Surface surface,
	in const Material material,
	in float3 V,
	out BsdfSample bsdf_sample,
	inout Random random

)
{
	bsdf_sample.bsdf_dir = 0;
	bsdf_sample.pdf = 0;
	bsdf_sample.bsdf = 0;

	// init surface params
	float3 N = surface.ffnormal;
	float3x3 tbn = get_tangent_space(N);
	
	float2 u2 = rand2(random);
	float3 L = importance_sample_cosweight(u2);
	L = tangent_to_world(L, tbn);
	L *= sign(dot(N, L));
	if (dot(N, L) > 0)
	{
		float3 diffuse = 0;
		float pdf = 0;
		evaluate_diffuse(material, L, V, N, diffuse, pdf);
		bsdf_sample.bsdf_dir = L;
		bsdf_sample.bsdf = diffuse;
		bsdf_sample.pdf = pdf;
	}
}

